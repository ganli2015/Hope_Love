#include "stdafx.h"
#include "AnalyzeChineseDictionary.h"
#include "FuncForTest.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"

#include "../DataCollection/Word.h"

#include "../Mind/Cerebrum.h"

#include "../MindInterface/iConcept.h"

using namespace Mind;
using namespace CommonTool;

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
	vector<WordDefinition> words;
	while (getline(in,line))
	{
		if(line=="") continue;

		WordDefinition wordDef;
		if (ExtractDefinition(line, wordDef))
		{
			words.push_back(wordDef);
		}
	}

	DistinguishBaseConceptAndNonBase(words);

	OutputBaseWords();
}

bool AnalyzeChineseDictionary::ExtractDefinition(const string line,WordDefinition& def) const
{
	if (CommonTool::FindChar(line, '*')!=line.cend()) return false;

	//Extract word.The word is in the brackets "¡¾...¡¿".
	string lbra = "¡¾";
	string rbra = "¡¿";
	auto leftBra = CommonTool::FindString(line, lbra);
	auto rightBra = CommonTool::FindString(line, rbra);
	string word = string(leftBra + lbra.length(), rightBra);

	def = WordDefinition(word);

	//Extract meanings.
	string indexOne = "£¨£±£©";
	string indexTwo = "£¨£²£©";
	string indexThree = "£¨£³£©";
	if (CommonTool::FindString(line, indexOne) == line.end())
	{
		//There is only one meaning.
		string meaning(rightBra + rbra.length(), line.end());
		def.AddMeaning(meaning);
	}
	else
	{
		//Extract first Three meanings.
		if(CommonTool::HasString(line, indexOne))
			def.AddMeaning(FindMeaning(line, indexOne));
		if (CommonTool::HasString(line, indexTwo))
			def.AddMeaning(FindMeaning(line, indexTwo));
		if (CommonTool::HasString(line, indexThree))
			def.AddMeaning(FindMeaning(line, indexThree));
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

void AnalyzeChineseDictionary::DistinguishBaseConceptAndNonBase(const vector<WordDefinition>& wordDefs)
{
	iCerebrum *brain = iCerebrum::Instance();
	auto allBaseConcepts = brain->GetAllBaseConcepts();

	//Find frequency of words in iCerebrum.
	vector<pair<string, long>> word_freq;
	set<string> addedWords;
	for (auto concept : allBaseConcepts)
	{
		long freq = 0;
		string word = concept->GetWord()->GetString();
		for (auto wordDef : wordDefs)
		{
			//Find if the word exists in Meaning of the word.
			if (MeaningHasWord(wordDef, concept->GetWord()))
			{
				freq += 1;
				wordDef.AddConcept(concept);
				addedWords.insert(word);
			}
		}

		//Add frequency to map.
		if (freq != 0)
		{
			if(addedWords.find(word)==addedWords.end())
			{
				word_freq.push_back(make_pair(word,1));
			}
			else
			{
				word_freq.push_back(make_pair(word, freq));
			}
		}
	}

	//Count words with most frequency.
	sort(word_freq.begin(), word_freq.end(), [=](std::pair<string, long>& a, std::pair<string, long>& b)
	{
		return a.second > b.second;
	}
	);
	
	map<string, WordDefinition> word_wordDef;
	for (auto wordDef : wordDefs)
	{
		word_wordDef[wordDef.GetWord()] = wordDef;
	}

	vector<WordDefinition> sortedByFreqWordDefs;
	for (auto word_freq_pair :word_freq)
	{
		string word = word_freq_pair.first;
		if (word_wordDef.count(word))
		{
			sortedByFreqWordDefs.push_back(word_wordDef[word]);
		}
	}

	_baseWordDefs = vector<WordDefinition>(sortedByFreqWordDefs.begin(), sortedByFreqWordDefs.begin() + BASE_COUNT);
	_baseWordDefs = vector<WordDefinition>(sortedByFreqWordDefs.begin()+ BASE_COUNT, sortedByFreqWordDefs.end());
}

bool AnalyzeChineseDictionary::MeaningHasWord(const WordDefinition& wordDef, const shared_ptr<DataCollection::Word> word) const
{
	for (auto meaning : wordDef.GetMeanings())
	{
		if (CommonTool::HasString(meaning, word->GetString()))
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
		out << baseDef.GetWord() << endl;
	}

	out.flush();
	out.close();
}
