#include "StdAfx.h"
#include "SpeakCommand.h"

#include "../DataCollection/Sentence.h"
#include "../DataCollection/LanguageFunc.h"

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

SpeakCommand::SpeakCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper)
{
	_datawrapper=datawrapper;
}

void SpeakCommand::Update()
{
	string sentence=_datawrapper->GetInputSentence();
	if(sentence=="") return;

	shared_ptr<SentenceParser> sentenceParser(new SentenceParser(sentence));
	sentenceParser->Execute();

	shared_ptr<DataCollection::Sentence> parsedSentence=sentenceParser->GetParsedSentence();
	shared_ptr<Mind::iConceptInteractTable> interactTable=sentenceParser->GetInteractTable();
	_datawrapper->AddParsedInputSentence(parsedSentence);

	//Output POS tagging as result.
	auto posTagging = ConvertToPOSString(sentenceParser->GetPOSTagging());
	_datawrapper->AddOutputSentence(posTagging);
	return;

	vector<shared_ptr<DataCollection::Sentence>> reactInputSentence;
	reactInputSentence.push_back(parsedSentence);
	shared_ptr<SpeakReaction> reaction(new SpeakReaction(reactInputSentence,interactTable));
	reaction->React();
	shared_ptr<DataCollection::Sentence> reactSentence=reaction->GetReactSentence();
	if(reactSentence==NULL)
	{
		//Add nothing to output when we have no react sentence.
		_datawrapper->AddOutputSentence("");
		return;
	}
	string reactStr=reactSentence->GetString();

	_datawrapper->AddOutputSentence(reactStr);
}

std::string SpeakCommand::ConvertToPOSString(const vector<shared_ptr<DataCollection::Word>>& words) const
{
	string res = "";
	for (auto word : words)
	{
		res += word->GetString() + "/" + DataCollection::LanguageFunc::GetChineseTern(word->Type()) + " ";
	}

	return res;
}
