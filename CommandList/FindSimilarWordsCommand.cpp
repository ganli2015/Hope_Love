#include "stdafx.h"
#include "FindSimilarWordsCommand.h"

#include "../SentenceAnalysisAlgorithm/FindSimilarConcept.h"

#include "../DataWrapperCPP/DataWrapper.h"

#include "../CommonTools/LogWriter.h"

FindSimilarConcept * FindSimilarWordsCommand::findSimilarConcept = NULL;

FindSimilarWordsCommand::FindSimilarWordsCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper) :
	SpeakCommand(datawrapper)
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

	if (findSimilarConcept == NULL)
	{
		findSimilarConcept = new FindSimilarConcept();
	}

	try
	{
		auto similar = findSimilarConcept->Find(sentence);
		LOG("Finish finding similar words.");
		auto output = ConstructOutput(similar);
		LOG("Finish ConstructOutput.");
		LOG(output);
		_datawrapper->AddOutputSentence(output);
	}
	catch (const std::exception& ex)
	{
		LOG_EXCEPTION(ex);
	}
	
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
