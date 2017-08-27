#pragma once
#include "SpeakCommand.h"


class POSTaggingCommand : public SpeakCommand
{
public:
	POSTaggingCommand(DataWrapperCPP::DataWrapper_Sentence* datawrapper);
	~POSTaggingCommand();

	virtual void Update();

private:

	string ConvertToPOSString(const vector<shared_ptr<DataCollection::Word>>& words) const;

};

