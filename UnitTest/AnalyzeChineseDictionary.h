#pragma once
#include "PublicHeader.h"


namespace DataCollection
{
	class Word;
}

namespace Mind
{
	class iConcept;
}

class WordMeaning
{
	string _meaning;
	vector<string> _segmentedMeaning;

public:
	WordMeaning(const string val):_meaning(val) {};
	~WordMeaning() {};

	string GetMeaning() const { return _meaning; }

	void SetSegmented(const vector<string>& val) { _segmentedMeaning = val; }

	bool HasWord(const string word)
	{
		return find(_segmentedMeaning.begin(), _segmentedMeaning.end(), word) != _segmentedMeaning.end();
	}
};


class WordDefinition
{
	string _word;
	vector<WordMeaning> _meanings;
	long _freq = 0;

	set<shared_ptr<WordDefinition>> _dependent;

public:
	WordDefinition() {};
	WordDefinition(string w) :_word(w) {};
	~WordDefinition() {};

	void AddMeaning(const string meaning) { _meanings.push_back(WordMeaning(meaning)); }

	vector<string> GetMeanings() const
	{
		vector<string> res(_meanings.size());
		transform(_meanings.begin(), _meanings.end(), res.begin(),
			[](const WordMeaning& meaning)->string {return meaning.GetMeaning(); });

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	//Pass one segmented meaning to me.
	//Each string in <segment> is a word.
	//////////////////////////////////////////////////////////////////////////
	void SetMeaningSegment(const string meaningStr, const vector<string>& segment)
	{
		auto existMeaning = find_if(_meanings.begin(), _meanings.end(),
			[meaningStr](const WordMeaning& meaning)->bool {return meaning.GetMeaning() == meaningStr; });

		if (existMeaning != _meanings.end())
		{
			existMeaning->SetSegmented(segment);
		}
		else
		{
			cout << "Cannot find meaning!!" << endl;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Check if there is a word in the meaning of me.
	//////////////////////////////////////////////////////////////////////////
	bool HasWordInMeaning(const string word)
	{
		for (auto meaning : _meanings)
		{
			if (meaning.HasWord(word))
			{
				return true;
			}
		}

		return false;
	}
	
	string GetWord() const { return _word; };

	long Freq() const { return _freq; }
	void Freq(long val) { _freq = val; }

	void AddDependence(const shared_ptr<WordDefinition> wordDef) { _dependent.insert(wordDef); }

	void Write(ofstream& out)
	{
		out << _word << " ";
		for (auto dep : _dependent)
		{
			out << dep->GetWord() << " ";
		}
		out << endl;
	}
};

class AnalyzeChineseDictionary
{
	//The count of base concepts for extracted word from the file.
	const int BASE_COUNT = 1000;
	//The count of base concepts for word with single character.
	const int BASE_COUNT_SINGLE_CHAR = 500;
	//The file path of extracted base words.
	const string BASE_WORD_FILE = "base_words.txt";
	//The file path of extracted base words.
	const string NON_BASE_WORD_FILE = "non_base_words.txt";

	const string WORD_CONNECTION_FILE = "word_connection.txt";

	map<string, shared_ptr<WordDefinition>> _allWordDefs;

	//Base words in Both Chinese Dictionary and iCerebrum.
	map<string,shared_ptr<WordDefinition>> _baseWordDefs;
	//Non Base words in Both Chinese Dictionary and iCerebrum.
	map<string, shared_ptr<WordDefinition>> _nonBaseWordDefs;

public:
	AnalyzeChineseDictionary();
	~AnalyzeChineseDictionary();

	//////////////////////////////////////////////////////////////////////////
	//Analyze file of Chinese dictionary and extract base words in it.
	//The base words are output to a file.
	//////////////////////////////////////////////////////////////////////////
	void ExtractBaseWords(const string filePath);

	//////////////////////////////////////////////////////////////////////////
	//Analyze file of Chinese dictionary and read base words from the file.
	//Then try to find the connection of non base words to other words.
	//Run <ExtractBaseWords> FIRST!
	//////////////////////////////////////////////////////////////////////////
	void BuildConnection(const string filePath);

private:

	void ExtractAllDefinitions(const string filePath);

	bool ExtractDefinition(const string line, shared_ptr<WordDefinition>& def) const;

	string FindMeaning(const string line, const string indexTag) const;

	void DistinguishBaseConceptAndNonBase();

	//////////////////////////////////////////////////////////////////////////
	//Generate a map with single character string as key and word definition as value.
	//It contains all word definitions.
	//////////////////////////////////////////////////////////////////////////
	multimap<string, shared_ptr<WordDefinition>> GenerateCharacterWordDefMap() const;

	//////////////////////////////////////////////////////////////////////////
	//Split meaning sentences into characters.
	//No duplicated!
	//////////////////////////////////////////////////////////////////////////
	set<string> GenerateCharacter(const shared_ptr<WordDefinition> line) const;

	long ComputeFreq(const string word, multimap<string, shared_ptr<WordDefinition>>& wordDefMap) const;

	bool MeaningHasWord(const shared_ptr<WordDefinition>& wordDef, const string word) const;

	//////////////////////////////////////////////////////////////////////////
	//Remove example sentence.
	//////////////////////////////////////////////////////////////////////////
	string ExtractMeaning(const string& line) const;

	void OutputBaseWords(const string filePath, const map<string, shared_ptr<WordDefinition>>& wordMap) const;

	//////////////////////////////////////////////////////////////////////////
	//Read base or non base word from the file and append to wordMap.
	//////////////////////////////////////////////////////////////////////////
	void ReadWordsFromFile(const string filePath, map<string, shared_ptr<WordDefinition>>& wordMap);

	//////////////////////////////////////////////////////////////////////////
	//WordSegment for each meaning of each word definition.
	//The result is in the object of word definition.
	//////////////////////////////////////////////////////////////////////////
	void SegmentMeanings();

	void FindConnection();

	void FindWhoDependOnMe(const string word, const shared_ptr<WordDefinition> wordDef, const multimap<string, shared_ptr<WordDefinition>>& wordDefMap);

	void OutputWordConnection() const;
};

