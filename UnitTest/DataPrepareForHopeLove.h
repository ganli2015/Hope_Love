#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

namespace Mind
{
	class GrammarSet;
	class ConceptDatabase;
}

class DataPrepareForHopeLove
{
public:
	DataPrepareForHopeLove() {};
	~DataPrepareForHopeLove() {};

	static void InitializeWeightsForGrammarSet(const string sampleFile, Mind::GrammarSet* grammarSet);

	static void ChangePrimaryKeyToHash(Mind::ConceptDatabase* db);
	static void ReadConceptConnectionFromFile(const string filePath, Mind::ConceptDatabase* db);
	static void RefreshConceptConnectionInConceptTable(Mind::ConceptDatabase* db);

private:

};

