#pragma once
#include "SpeakCommand.h"


class FindSimilarWordsCommand : public SpeakCommand
{
public:
	FindSimilarWordsCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);
	~FindSimilarWordsCommand();

	virtual void Update();

private:

	string ConstructOutput(const vector<string>& similarWords) const;
};

