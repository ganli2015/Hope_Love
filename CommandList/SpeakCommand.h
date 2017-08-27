#pragma once
#include "command.h"

namespace DataCollection
{
	class Word;
}

class _COMMANDINOUT SpeakCommand :
	public Command
{
	typedef typename std::function < Command*(DataWrapperCPP::DataWrapper_Sentence*)> CommandCreator ;
	static map<string, CommandCreator> commandMap;
protected:
	DataWrapperCPP::DataWrapper_Sentence* _datawrapper;

public:
	~SpeakCommand(void);
	SpeakCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);

	virtual void Update() = 0;

	static Command* Create(const string commandName , DataWrapperCPP::DataWrapper_Sentence* data);

private:
	static map<string, SpeakCommand::CommandCreator> CreateCommandMap();

};

