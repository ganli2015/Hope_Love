#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

namespace Mind
{
	class GrammarSet;
}

class DataPrepareForHopeLove
{
public:
	DataPrepareForHopeLove() {};
	~DataPrepareForHopeLove() {};

	static void InitializeWeightsForGrammarSet(const string sampleFile, Mind::GrammarSet* grammarSet);

private:

};

