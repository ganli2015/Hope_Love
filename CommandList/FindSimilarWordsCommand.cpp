#include "stdafx.h"
#include "FindSimilarWordsCommand.h"

#include "../SentenceAnalysisAlgorithm/FindSimilarConcept.h"

#include "../DataWrapperCPP/DataWrapper.h"

#include "../CommonTools/LogWriter.h"

FindSimilarWordsCommand::FindSimilarWordsCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper) :SpeakCommand(datawrapper)
{
}


FindSimilarWordsCommand::~FindSimilarWordsCommand()
{
}

void FindSimilarWordsCommand::Update()
{
	string sentence = _datawrapper->GetInputSentence();
	if (sentence == "") return;

	LOG_FORMAT("Word to find simialr: %s", sentence);

	FindSimilarConcept findConcept;
	auto similar = findConcept.Find(sentence);
	auto output = ConstructOutput(similar);
	_datawrapper->AddOutputSentence(output);
}

std::string FindSimilarWordsCommand::ConstructOutput(const vector<string>& similarWords) const
{
	string res = "";
	if (similarWords.empty())
	{
		res = "找不到相似的词语";
		return res;
	}
	
	for (auto similar : similarWords)
	{
		res += similar + " ";
	}

	return res;
}
