#include "stdafx.h"
#include "AnalyzeChineseDictionary.h"

#include "../CommonTools/CommonStringFunction.h"

AnalyzeChineseDictionary::AnalyzeChineseDictionary()
{
}


AnalyzeChineseDictionary::~AnalyzeChineseDictionary()
{
}

void AnalyzeChineseDictionary::Analyze(const string filePath)
{
	ifstream in(filePath);
	string line = "";

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
