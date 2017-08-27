#include "stdafx.h"
#include "POSTaggingCommand.h"

#include "../DataCollection/Sentence.h"
#include "../DataCollection/LanguageFunc.h"

#include "../DataWrapperCPP/DataWrapper.h"

#include "../ReactionList/SpeakCommand.h"

#include "../SentenceAnalysisAlgorithm/SentenceParser.h"

POSTaggingCommand::POSTaggingCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper) :SpeakCommand(datawrapper)
{
}


POSTaggingCommand::~POSTaggingCommand()
{
}

void POSTaggingCommand::Update()
{
	string sentence = _datawrapper->GetInputSentence();
	if (sentence == "") return;

	shared_ptr<SentenceParser> sentenceParser(new SentenceParser(sentence));
	sentenceParser->Execute();

	//Output POS tagging as result.
	auto posTagging = ConvertToPOSString(sentenceParser->GetPOSTagging());
	_datawrapper->AddOutputSentence(posTagging);
	return;
}

std::string POSTaggingCommand::ConvertToPOSString(const vector<shared_ptr<DataCollection::Word>>& words) const
{
	string res = "";
	for (auto word : words)
	{
		res += word->GetString() + "/" + DataCollection::LanguageFunc::GetChineseTern(word->Type()) + " ";
	}

	return res;
}