#include "stdafx.h"
#include "DataPrepareForHopeLove.h"

#include "../Mind/GrammarFeatureModel.h"
#include "../Mind/GrammarLocalModel.h"
#include "../Mind/GrammarPatternModel.h"
#include "../Mind/GrammarModel.h"
#include "../Mind/GrammaSet.h"
#include "../Mind/ConceptSet.h"
#include "../Mind/ConceptDatabase.h"

#include "../CommonTools/DBoperator.h"

#include "../DataCollection/Word.h"

using namespace Mind;

using namespace CommonTool;
using namespace FuncForTest;
using namespace DataCollection;

//These are preparations for Hope_Love, including training data and collecting data.

TEST(Preparation, GrammarFeatureTrainer_CollectGrammarFeatures)
{
	CHECK_SKIP;

	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	//Before running this test, clean data in table 
	GrammarFeatureTrainer trainer;
	trainer.CollectFeatures("E:\\Artificial Intelligence\\Document\\DataBase\\myconv_feature.txt");
}

TEST(Preparation, GrammarFeatureTrainer_ComputeWeights)
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	//Compute weights for different grammar feature templates.
	//Before running this test, run <CollectGrammarFeatures> first to collect all feature from a sample file.
	GrammarFeatureTrainer trainer;
	trainer.ComputeWeights("E:\\Artificial Intelligence\\Document\\DataBase\\myconv_train.txt");
}

TEST(Preparation, GrammarModelTrainer_OptimizeWeights)
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	GrammarSet *grammarSet = new GrammarSet();
	grammarSet->InitializeGrammarModel();
	DataPrepareForHopeLove::InitializeWeightsForGrammarSet(
		"E:\\Artificial Intelligence\\Document\\DataBase\\myconv_train.txt", grammarSet);
}

TEST(Preparation, CollectNewBaseConcept)
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	ConceptSet conceptSet;
	conceptSet.CollectNewBaseConcepts("E:\\Artificial Intelligence\\Document\\DataBase\\myconv.txt");
}

TEST(Preparation, ChangePrimaryKeyToHash)
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	auto conceptDB = new Mind::ConceptDatabase;
	DataPrepareForHopeLove::ChangePrimaryKeyToHash(conceptDB);
	delete conceptDB;
}

TEST(Preparation, ReadConceptConnectionFromFile)
{
	//Read concept connection data from file,
	//and build connection table in the databases.

	CHECK_SKIP;
	auto conceptDB = new Mind::ConceptDatabase;
	conceptDB->Connect();
	DataPrepareForHopeLove::ReadConceptConnectionFromFile("D:\\Projects\\Hope_Love\\Mind\\HopeLoveData\\ConceptConnections_Initial.txt",conceptDB);
	delete conceptDB;
}

TEST(Preparation, RefreshConceptConnection)
{
	//Relate connection table with concept tables.

	CHECK_SKIP;
	auto conceptDB = new Mind::ConceptDatabase;
	conceptDB->Connect();
	DataPrepareForHopeLove::RefreshConceptConnectionInConceptTable(conceptDB);
	delete conceptDB;
}

TEST(Preparation, TestPOS)
{
	CHECK_SKIP;
	vector<shared_ptr<DataCollection::Word>> sentence;
	sentence.push_back(ToWord("Äã", Pronoun));
	sentence.push_back(ToWord("È¥", Pronoun));
	sentence.push_back(ToWord("ÄÄ", Pronoun));

	vector<shared_ptr<DataCollection::Word>> sentence2;
	sentence2.push_back(ToWord("Äã", Pronoun));
	sentence2.push_back(ToWord("È¥", Verb));
	sentence2.push_back(ToWord("ÄÄ", Pronoun));

	GrammarFeatureModel model;
	model.LoadAllFeatures();
	double res = model.ComputePossiblity(sentence);
	double res2 = model.ComputePossiblity(sentence2);
}

void DataPrepareForHopeLove::InitializeWeightsForGrammarSet(const string sampleFile, GrammarSet* grammarSet)
{
	grammarSet->InitializeWeights(sampleFile);
}

void DataPrepareForHopeLove::ChangePrimaryKeyToHash(Mind::ConceptDatabase* db)
{
	db->ChangePrimaryKeyToHash();
}

void DataPrepareForHopeLove::ReadConceptConnectionFromFile(const string filePath, Mind::ConceptDatabase* db)
{
	db->ReadConceptConnectionFromFile(filePath);
}

void DataPrepareForHopeLove::RefreshConceptConnectionInConceptTable(Mind::ConceptDatabase* db)
{
	db->RefreshConceptConnectionInConceptTable();
}

