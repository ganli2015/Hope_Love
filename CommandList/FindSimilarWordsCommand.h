#pragma once
#include "SpeakCommand.h"

class FindSimilarConcept;

class FindSimilarWordsCommand : public SpeakCommand
{
	static FindSimilarConcept *findSimilarConcept;

public:
	FindSimilarWordsCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);
	~FindSimilarWordsCommand();

	virtual void Update();

private:

	string ConstructOutput(const vector<string>& similarWords) const;
};

