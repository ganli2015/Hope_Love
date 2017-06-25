#include "stdafx.h"
#include "AnalyzeChineseDictionary.h"
#include "FuncForTest.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/Character.h"
#include "../DataCollection/LanguageFunc.h"

#include "../Mind/Cerebrum.h"

#include "../MindInterface/iConcept.h"

using namespace Mind;
using namespace CommonTool;
using namespace DataCollection;

AnalyzeChineseDictionary::AnalyzeChineseDictionary()
{
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	iCerebrum::SetInstance(Cerebrum::Instance());
}


AnalyzeChineseDictionary::~AnalyzeChineseDictionary()
{
}

void AnalyzeChineseDictionary::Analyze(const string filePath)
{
	ifstream in(filePath);
	string line = "";

	//Extract word and meanings from the file.
	vector<shared_ptr<WordDefinition>> words;
	while (getline(in,line))
	{
		if(line=="") continue;

		shared_ptr<WordDefinition> wordDef;
		if (ExtractDefinition(line, wordDef))
		{
			words.push_back(wordDef);
		}
	}

	DistinguishBaseConceptAndNonBase(words);

	OutputBaseWords();
}

bool AnalyzeChineseDictionary::ExtractDefinition(const string line, shared_ptr<WordDefinition>& def) const
{
	if (CommonTool::FindChar(line, '*')!=line.cend()) return false;

	//Extract word.The word is in the brackets "¡¾...¡¿".
	string lbra = "¡¾";
	string rbra = "¡¿";
	auto leftBra = CommonTool::FindString(line, lbra);
	auto rightBra = CommonTool::FindString(line, rbra);
	string word = string(leftBra + lbra.length(), rightBra);

	def = make_shared<WordDefinition>(word);

	//Extract meanings.
	string indexOne = "£¨£±£©";
	string indexTwo = "£¨£²£©";
	string indexThree = "£¨£³£©";
	if (CommonTool::FindString(line, indexOne) == line.end())
	{
		//There is only one meaning.
		string meaning(rightBra + rbra.length(), line.end());
		def->AddMeaning(meaning);
	}
	else
	{
		//Extract first Three meanings.
		if(CommonTool::HasString(line, indexOne))
			def->AddMeaning(FindMeaning(line, indexOne));
		if (CommonTool::HasString(line, indexTwo))
			def->AddMeaning(FindMeaning(line, indexTwo));
		if (CommonTool::HasString(line, indexThree))
			def->AddMeaning(FindMeaning(line, indexThree));
	}

	return true;
}

std::string AnalyzeChineseDictionary::FindMeaning(const string line, const string indexTag) const
{
	//Select string between two indices brackets.
	auto indexLoc = CommonTool::FindString(line, indexTag);
	auto nextBraLoc= CommonTool::FindString(line, indexLoc + indexTag.length(),"£¨");

	return string(indexLoc + indexTag.length(), nextBraLoc);
}

void AnalyzeChineseDictionary::DistinguishBaseConceptAndNonBase(const vector<shared_ptr<WordDefinition>>& wordDefs)
{
	iCerebrum *brain = iCerebrum::Instance();
	auto allBaseConcepts = brain->GetAllBaseConcepts();

	//Generate a map with single character string as key and word definition as value.
	//The map makes convenience for querying concept frequency.
	multimap<string, shared_ptr<WordDefinition>> char_wordDefs;
	for (auto wordDef : wordDefs)
	{
		try
		{
			auto chars = GenerateCharacter(wordDef);
			for (auto ch : chars)
			{
				char_wordDefs.insert(make_pair(ch, wordDef));
			}
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}


	//Find frequency of words in iCerebrum.
	vector<pair<string, long>> word_freq;
	set<string> addedWord;
	for (auto concept : allBaseConcepts)
	{
		string word = concept->GetWord()->GetString();
		if (addedWord.find(word) != addedWord.end())
		{
			cout << "Duplicated word: " + word << endl;
			continue;
		}

		long freq = ComputeFreq(word,char_wordDefs);

		if (freq != 0)
		{
			word_freq.push_back(make_pair(word, freq));
			addedWord.insert(word);
		}
	}

	//Count words with most frequency.
	sort(word_freq.begin(), word_freq.end(), [=](std::pair<string, long>& a, std::pair<string, long>& b)
	{
		return a.second > b.second;
	}
	);
	
	map<string, shared_ptr<WordDefinition>> word_wordDef;
	for (auto wordDef : wordDefs)
	{
		word_wordDef[wordDef->GetWord()] = wordDef;
	}

	vector<shared_ptr<WordDefinition>> sortedByFreqWordDefs;
	for (auto word_freq_pair :word_freq)//<word_freq> is sorted by frequency.
	{
		string word = word_freq_pair.first;
		if (word_wordDef.count(word))
		{
			auto wordDef = word_wordDef[word];
			wordDef->Freq(word_freq_pair.second);
			sortedByFreqWordDefs.push_back(wordDef);
		}
	}

	_baseWordDefs = vector<shared_ptr<WordDefinition>>(sortedByFreqWordDefs.begin(), sortedByFreqWordDefs.begin() + BASE_COUNT);
	_nonBaseWordDefs = vector<shared_ptr<WordDefinition>>(sortedByFreqWordDefs.begin()+ BASE_COUNT, sortedByFreqWordDefs.end());
}

set<string> AnalyzeChineseDictionary::GenerateCharacter(const shared_ptr<WordDefinition> line)
{
	set<string> res;
	for (auto meaning : line->GetMeanings())
	{
		auto chars = LanguageFunc::ConvertStringToCharacter(meaning);
		for (auto ch : chars)
		{
			res.insert(ch->GetString());
		}
	}

	return res;
}

long AnalyzeChineseDictionary::ComputeFreq(const string word, multimap<string, shared_ptr<WordDefinition>>& wordDefMap) const
{
	auto chars = LanguageFunc::ConvertStringToCharacter(word);
	//WordDefinition that contains one of <chars>.
	set<shared_ptr<WordDefinition>> relatedWordDefs;
	for (auto ch : chars)
	{
		if (wordDefMap.find(ch->GetString()) == wordDefMap.end()) continue;

		//Find all word definition containing <ch>.
		auto allDefs = FindAll(ch->GetString(), wordDefMap);

		relatedWordDefs.insert(allDefs.begin(), allDefs.end());
	}

	long freq = 0;
	for (auto wordDef : relatedWordDefs)
	{
		if (MeaningHasWord(wordDef, word))
		{
			freq++;
		}
	}

	return freq;
}

bool AnalyzeChineseDictionary::MeaningHasWord(const shared_ptr<WordDefinition>& wordDef, const string word) const
{
	for (auto meaning : wordDef->GetMeanings())
	{
		if (CommonTool::HasString(meaning, word))
		{
			return true;
		}
	}

	return false;
}

void AnalyzeChineseDictionary::OutputBaseWords() const
{
	ofstream out("base_words.txt");

	for (auto baseDef : _baseWordDefs)
	{
		out << baseDef->GetWord() << "  " << baseDef->Freq() << endl;
	}

	out.flush();
	out.close();
}
