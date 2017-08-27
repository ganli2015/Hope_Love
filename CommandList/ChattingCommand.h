#pragma once
#include "SpeakCommand.h"

class ChattingCommand : public SpeakCommand
{
public:
	ChattingCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);
	~ChattingCommand();

	virtual void Update() ;
};

