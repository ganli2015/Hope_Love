#include "StdAfx.h"
#include "SpeakCommand.h"
#include "ChattingCommand.h"
#include "POSTaggingCommand.h"

using namespace std;

map<string, SpeakCommand::CommandCreator> SpeakCommand::commandMap = CreateCommandMap();

SpeakCommand::~SpeakCommand(void)
{
}

SpeakCommand::SpeakCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper)
{
	_datawrapper=datawrapper;
}

Command* SpeakCommand::Create(const string commandName, DataWrapperCPP::DataWrapper_Sentence* data)
{
	if (commandMap.find(commandName) == commandMap.end())
	{
		throw invalid_argument("Cannot find command with name : " + commandName);
	}

	return commandMap[commandName](data);
}

map<string, SpeakCommand::CommandCreator> SpeakCommand::CreateCommandMap()
{
	map<string, CommandCreator> res;

	res["chatting"] = [](DataWrapperCPP::DataWrapper_Sentence* data)->
		Command* {return new ChattingCommand(data); };

	res["posTagging"] = [](DataWrapperCPP::DataWrapper_Sentence* data)->
		Command* {return new POSTaggingCommand(data); };

	return res;
}
