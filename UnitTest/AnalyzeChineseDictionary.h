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

	vector<shared_ptr<Mind::iConcept>> _concepts;

public:
	WordDefinition() {};
	WordDefinition(string w) :_word(w) {};
	~WordDefinition() {};

	void AddMeaning(const string meaning) { _meanings.push_back(meaning); }

	vector<string> GetMeanings() const { return _meanings; }

	void AddConcept(const shared_ptr<Mind::iConcept> concept) { _concepts.push_back(concept); }
	
	string GetWord() const { return _word; };
};

class AnalyzeChineseDictionary
{
	const int BASE_COUNT = 1000;

	vector<WordDefinition> _baseWordDefs;
	vector<WordDefinition> _nonBaseWordDefs;

public:
	AnalyzeChineseDictionary();
	~AnalyzeChineseDictionary();

	void Analyze(const string filePath);

private:

	bool ExtractDefinition(const string line, WordDefinition& def) const;

	string FindMeaning(const string line, const string indexTag) const;

	void DistinguishBaseConceptAndNonBase(const vector<WordDefinition>& wordDefs);

	bool MeaningHasWord(const WordDefinition& wordDef, const shared_ptr<DataCollection::Word> word) const;

	void OutputBaseWords() const;
};

