#include "stdafx.h"
#include "DataPrepareForHopeLove.h"

#include "../Mind/GrammarFeatureModel.h"

#include "../CommonTools/DBoperator.h"

#include "../DataCollection/Word.h"

using namespace Mind;

using namespace CommonTool;
using namespace FuncForTest;
using namespace DataCollection;

//These are preparations for Hope_Love, including training data and collecting data.

TEST(DISABLED_Preparation, CollectGrammarFeatures)
{
	GrammarFeatureTrainer trainer;
	trainer.CollectFeatures("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_wo_pos.txt");
}

TEST(DISABLED_Preparation, ComputeWeights)
{
	GrammarFeatureTrainer trainer;
	trainer.ComputeWeights("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_de_sub.txt");
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