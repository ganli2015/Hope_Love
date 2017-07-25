#include "stdafx.h"
#include "AnalyzeChineseDictionary.h"
#include "FuncForTest.h"
#include "TestPOSTagging.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"
#include "../CommonTools/LogWriter.h"

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

void AnalyzeChineseDictionary::ExtractBaseWords(const string filePath)
{
	ExtractAllDefinitions(filePath);

	DistinguishBaseConceptAndNonBase();

	OutputBaseWords(BASE_WORD_FILE,_baseWordDefs);
	OutputBaseWords(NON_BASE_WORD_FILE, _nonBaseWordDefs);
}

void AnalyzeChineseDictionary::BuildConnection(const string filePath)
{
	//TODO: Segment each meaning first and then find connections.
	ExtractAllDefinitions(filePath);

	ReadWordsFromFile(BASE_WORD_FILE, _baseWordDefs);

	SegmentMeanings();

	FindConnection();

	OutputWordConnection();
}

void AnalyzeChineseDictionary::ExtractAllDefinitions(const string filePath)
{
	ifstream in(filePath);
	string line = "";

	//Extract word and meanings from the file.
	while (getline(in, line))
	{
		if (line == "") continue;

		shared_ptr<WordDefinition> wordDef;
		if (ExtractDefinition(line, wordDef))
		{
			_allWordDefs[wordDef->GetWord()] = (wordDef);
		}
	}
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
		def->AddMeaning(ExtractMeaning(meaning));
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

	string wholeMeaning= string(indexLoc + indexTag.length(), nextBraLoc);
	return ExtractMeaning(wholeMeaning);
}

void AnalyzeChineseDictionary::DistinguishBaseConceptAndNonBase()
{
	iCerebrum *brain = iCerebrum::Instance();
	auto allBaseConcepts = brain->GetAllBaseConcepts();

	//Generate a map with single character string as key and word definition as value.
	//The map makes convenience for querying concept frequency.
	multimap<string, shared_ptr<WordDefinition>> char_wordDefs = GenerateCharacterWordDefMap();

	//Find frequency of words in iCerebrum.
	vector<pair<string, long>> word_freq;
	set<string> addedWord;
	for (auto concept : allBaseConcepts)
	{
		string word = concept->GetWord()->GetString();
		if (addedWord.find(word) != addedWord.end())
		{
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

	//Sort word definitions by frequency according to <word_freq>.
	vector<pair<string,shared_ptr<WordDefinition>>> sortedByFreqWordDefs;
	for (auto word_freq_pair :word_freq)//<word_freq> is sorted by frequency.
	{
		string word = word_freq_pair.first;
		if (_allWordDefs.count(word))
		{
			auto wordDef = _allWordDefs[word];
			wordDef->Freq(word_freq_pair.second);
			sortedByFreqWordDefs.push_back(make_pair(word,wordDef));
		}
	}

	_baseWordDefs.insert(sortedByFreqWordDefs.begin(), sortedByFreqWordDefs.begin() + BASE_COUNT);
	_nonBaseWordDefs.insert(sortedByFreqWordDefs.begin()+ BASE_COUNT, sortedByFreqWordDefs.end());


	//Count frequency of word with single character.
	vector<pair<string, long>> singleChar_freq;
	for (auto concept : allBaseConcepts)
	{
		auto word = concept->GetWord();
		if(word->Type()==DataCollection::Punctuation) continue;
		if (word->NumOfChara() == 1)
		{
			long freq = ComputeFreq(word->GetString(), char_wordDefs);
			singleChar_freq.push_back(make_pair(word->GetString(),freq));
		}
	}

	//Sort <singleChar_freq>.
	sort(singleChar_freq.begin(), singleChar_freq.end(), [=](std::pair<string, long>& a, std::pair<string, long>& b)
	{
		return a.second > b.second;
	}
	);

	//Append to <_baseWordDefs>.
	for (int i=0;i<BASE_COUNT_SINGLE_CHAR;++i)
	{
		shared_ptr<WordDefinition> wordDef = make_shared<WordDefinition>(singleChar_freq[i].first);
		wordDef->Freq(singleChar_freq[i].second);
		_baseWordDefs[singleChar_freq[i].first] = wordDef;
	}
}

multimap<string, shared_ptr<WordDefinition>> AnalyzeChineseDictionary::GenerateCharacterWordDefMap() const
{
	multimap<string, shared_ptr<WordDefinition>> char_wordDefs;
	for (auto wordDef : _allWordDefs)
	{
		try
		{
			auto chars = GenerateCharacter(wordDef.second);
			for (auto ch : chars)
			{
				char_wordDefs.insert(make_pair(ch, wordDef.second));
			}
		}
		catch (const std::exception& ex)
		{
			continue;
		}
	}

	return char_wordDefs;
}

set<string> AnalyzeChineseDictionary::GenerateCharacter(const shared_ptr<WordDefinition> line) const
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

std::string AnalyzeChineseDictionary::ExtractMeaning(const string& line) const
{
	auto beforeColon = CommonTool::FindString(line, "£º");
	return string(line.begin(), beforeColon);
}

void AnalyzeChineseDictionary::OutputBaseWords(const string filePath,const map<string, shared_ptr<WordDefinition>>& wordMap) const
{
	ofstream out(filePath);

	for (auto baseDef : wordMap)
	{
		out << baseDef.first << "  " << baseDef.second->Freq() << endl;
	}

	out.flush();
	out.close();
}

void AnalyzeChineseDictionary::ReadWordsFromFile(const string filePath, map<string, shared_ptr<WordDefinition>>& wordMap)
{
	ifstream in(filePath);
	string line = "";
	while (getline(in, line))
	{
		if (line == "") continue;

		auto split = SplitString(line, ' ');
		CommonTool::RemoveEmptyString(split);
		Check(split.size() == 2);

		auto word = split.front();

		//Append word to wordMap.
		//Find word in <_allWordDefs>.
		if (_allWordDefs.find(word) != _allWordDefs.end())
		{
			auto wordDef = _allWordDefs[word];
			wordMap[word] = wordDef;
		}
		else
		{
			//Create a new one.
			auto newWordDef = make_shared<WordDefinition>(word);
			wordMap[word] = newWordDef;
		}
	}
}

void AnalyzeChineseDictionary::SegmentMeanings()
{
	int count = 0;

	TestPOSTagging posTagging;
	for (auto wordDef : _allWordDefs)
	{
		LOG_FORMAT("Word Definition: %s", wordDef.second->GetWord().c_str());
		for (auto meaning : wordDef.second->GetMeanings())
		{
			if (meaning == "") continue;
			//Filter long meaning.
			if(meaning.size()>30) continue;

			try
			{
				LOG_FORMAT("Begin segment: %s", meaning.c_str());
				auto segment = posTagging.SegmentWord(meaning);
				wordDef.second->SetMeaningSegment(meaning, segment);
			}
			catch (const std::exception&)
			{
				ERROR_FORMAT("Fail to segment meaning: %s.", meaning.c_str());
			}

		}

		++count;
		if (count % 1000 == 0)
		{
			LOG_FORMAT("Have segmented: %d", count);
		}
	}
}

void AnalyzeChineseDictionary::FindConnection()
{
	//Generate a map in which key is single Character and value is related word definition.
	//And word definitions come from <_allWordDefs>.
	auto char_wordDef_map = GenerateCharacterWordDefMap();

	for (auto pair : _allWordDefs)
	{
		string word = pair.first;
		auto wordDef = pair.second;

		try
		{
			FindWhoDependOnMe(word, wordDef, char_wordDef_map);
		}
		catch (const std::exception&)
		{
			ERROR_FORMAT("Fail to handle word: %s.", word.c_str());
		}
	}

	for (auto pair : _baseWordDefs)
	{
		string word = pair.first;
		auto wordDef = pair.second;

		try
		{
			FindWhoDependOnMe(word, wordDef, char_wordDef_map);
		}
		catch (const std::exception&)
		{
			ERROR_FORMAT("Fail to handle word: %s.", word.c_str());
		}
	}
}

void AnalyzeChineseDictionary::FindWhoDependOnMe(const string word, 
	const shared_ptr<WordDefinition> wordDef,
	const multimap<string, shared_ptr<WordDefinition>>& wordDefMap)
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

	//Find if <relatedWordDefs> contains <word>
	for (auto related : relatedWordDefs)
	{
		for (auto meaning : related->GetMeanings())
		{
			if (FindString(meaning, word) != meaning.end())
			{
				//TODO : Check for each meaning respectively.
				//Different meanings correspond to Different concepts.
				related->AddDependence(wordDef);
				break;
			}
		}
	}
}

void AnalyzeChineseDictionary::OutputWordConnection() const
{
	ofstream out(WORD_CONNECTION_FILE);

	for (auto pair : _allWordDefs)
	{
		pair.second->Write(out);
	}

	out.flush();
	out.close();
}
