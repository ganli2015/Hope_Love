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

class WordDefinition
{
	string _word;
	vector<string> _meanings;
	long _freq = 0;

public:
	WordDefinition() {};
	WordDefinition(string w) :_word(w) {};
	~WordDefinition() {};

	void AddMeaning(const string meaning) { _meanings.push_back(meaning); }

	vector<string> GetMeanings() const { return _meanings; }
	
	string GetWord() const { return _word; };

	long Freq() const { return _freq; }
	void Freq(long val) { _freq = val; }
};

class AnalyzeChineseDictionary
{
	//The count of base concepts for extracted word from the file.
	const int BASE_COUNT = 1000;
	//The count of base concepts for word with single character.
	const int BASE_COUNT_SINGLE_CHAR = 500;

	map<string, shared_ptr<WordDefinition>> _allWordDefs;

	//Base words in Both Chinese Dictionary and iCerebrum.
	map<string,shared_ptr<WordDefinition>> _baseWordDefs;
	//Non Base words in Both Chinese Dictionary and iCerebrum.
	map<string, shared_ptr<WordDefinition>> _nonBaseWordDefs;

public:
	AnalyzeChineseDictionary();
	~AnalyzeChineseDictionary();

	void Analyze(const string filePath);

private:

	bool ExtractDefinition(const string line, shared_ptr<WordDefinition>& def) const;

	string FindMeaning(const string line, const string indexTag) const;

	void DistinguishBaseConceptAndNonBase();

	//////////////////////////////////////////////////////////////////////////
	//Split meaning sentences into characters.
	//No duplicated!
	//////////////////////////////////////////////////////////////////////////
	set<string> GenerateCharacter(const shared_ptr<WordDefinition> line);

	long ComputeFreq(const string word, multimap<string, shared_ptr<WordDefinition>>& wordDefMap) const;

	bool MeaningHasWord(const shared_ptr<WordDefinition>& wordDef, const string word) const;

	//////////////////////////////////////////////////////////////////////////
	//Remove example sentence.
	//////////////////////////////////////////////////////////////////////////
	string ExtractMeaning(const string& line) const;

	void OutputBaseWords() const;
};

