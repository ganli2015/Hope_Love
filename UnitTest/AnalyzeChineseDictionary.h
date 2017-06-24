#pragma once
#include "PublicHeader.h"


class WordDefinition
{
	string _word;
	vector<string> _meanings;

public:
	WordDefinition() {};
	WordDefinition(string w) :_word(w) {};
	~WordDefinition() {};

	void AddMeaning(const string meaning) { _meanings.push_back(meaning); }
};

class AnalyzeChineseDictionary
{
public:
	AnalyzeChineseDictionary();
	~AnalyzeChineseDictionary();

	void Analyze(const string filePath);

private:

	bool ExtractDefinition(const string line, WordDefinition& def) const;

	string FindMeaning(const string line, const string indexTag) const;
};

