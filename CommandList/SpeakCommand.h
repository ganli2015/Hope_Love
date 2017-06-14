#pragma once
#include "command.h"

namespace DataCollection
{
	class Word;
}

class _COMMANDINOUT SpeakCommand :
	public Command
{
	DataWrapperCPP::DataWrapper_Sentence* _datawrapper;

public:
	SpeakCommand(void);
	~SpeakCommand(void);
	SpeakCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);

	void Update();

private:

	string ConvertToPOSString(const vector<shared_ptr<DataCollection::Word>>& words) const;
};

