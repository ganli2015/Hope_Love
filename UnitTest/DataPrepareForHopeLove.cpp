#include "stdafx.h"
#include "DataPrepareForHopeLove.h"

#include "../Mind/GrammarFeatureModel.h"
#include "../Mind/GrammarLocalModel.h"
#include "../Mind/GrammarPatternModel.h"
#include "../Mind/GrammarModel.h"
#include "../Mind/GrammaSet.h"

#include "../CommonTools/DBoperator.h"

#include "../DataCollection/Word.h"

using namespace Mind;

using namespace CommonTool;
using namespace FuncForTest;
using namespace DataCollection;

//These are preparations for Hope_Love, including training data and collecting data.

TEST(DISABLED_Preparation, GrammarFeatureTrainer_CollectGrammarFeatures)
{
	//Before running this test, clean data in table 
	GrammarFeatureTrainer trainer;
	trainer.CollectFeatures("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_wo_pos.txt");
}

TEST(DISABLED_Preparation, GrammarFeatureTrainer_ComputeWeights)
{
	//Compute weights for different grammar feature templates.
	//Before running this test, run <CollectGrammarFeatures> first to collect all featuer from a sample file.
	GrammarFeatureTrainer trainer;
	trainer.ComputeWeights("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_de_sub.txt");
}

TEST(Preparation, GrammarModelTrainer_OptimizeWeights)
{
	GrammarSet *grammarSet = new GrammarSet();
	grammarSet->InitializeGrammarModel();
	DataPrepareForHopeLove::InitializeWeightsForGrammarSet(
		"E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_de_sub.txt", grammarSet);
}

TEST(DISABLED_Preparation, TestPOS)
{
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
