#include "stdafx.h"
#include "ChattingCommand.h"

#include "../DataCollection/Sentence.h"
#include "../DataCollection/LanguageFunc.h"

#include "../DataWrapperCPP/DataWrapper.h"

#include "../ReactionList/SpeakCommand.h"

#include "../SentenceAnalysisAlgorithm/SentenceParser.h"

ChattingCommand::ChattingCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper): SpeakCommand(datawrapper)
{

}

ChattingCommand::~ChattingCommand()
{
}

void ChattingCommand::Update()
{
	string sentence = _datawrapper->GetInputSentence();
	if (sentence == "") return;

	shared_ptr<SentenceParser> sentenceParser(new SentenceParser(sentence));
	sentenceParser->Execute();

	shared_ptr<DataCollection::Sentence> parsedSentence = sentenceParser->GetParsedSentence();
	shared_ptr<Mind::iConceptInteractTable> interactTable = sentenceParser->GetInteractTable();
	_datawrapper->AddParsedInputSentence(parsedSentence);

	vector<shared_ptr<DataCollection::Sentence>> reactInputSentence;
	reactInputSentence.push_back(parsedSentence);
	shared_ptr<SpeakReaction> reaction(new SpeakReaction(reactInputSentence, interactTable));
	reaction->React();
	shared_ptr<DataCollection::Sentence> reactSentence = reaction->GetReactSentence();
	if (reactSentence == NULL)
	{
		//Add nothing to output when we have no react sentence.
		_datawrapper->AddOutputSentence("");
		return;
	}
	string reactStr = reactSentence->GetString();

	_datawrapper->AddOutputSentence(reactStr);
}
