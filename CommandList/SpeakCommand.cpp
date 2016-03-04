#include "StdAfx.h"
#include "SpeakCommand.h"

#include "../DataWrapperCPP/DataWrapper.h"

#include "../ReactionList/SpeakCommand.h"

#include "../SentenceAnalysisAlgorithm/SentenceParser.h"

using namespace std;
SpeakCommand::SpeakCommand(void)
{
}


SpeakCommand::~SpeakCommand(void)
{
}

SpeakCommand::SpeakCommand(DataWrapperCPP::DataWrapper* datawrapper)
{
	_datawrapper=datawrapper;
}

void SpeakCommand::Update()
{
	string sentence=_datawrapper->GetInputSentence();
	shared_ptr<SentenceParser> sentenceParser(new SentenceParser(sentence));
	sentenceParser->Execute();

	shared_ptr<Reaction> reaction(new SpeakReaction(sentenceParser->GetParsedSentence()));
	reaction->React();
}
