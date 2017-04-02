#include "stdafx.h"
#include "DataPrepareForHopeLove.h"

#include "../Mind/GrammarFeatureModel.h"

#include "../CommonTools/DBoperator.h"

using namespace Mind;

using namespace CommonTool;

//These are preparations for Hope_Love, including training data and collecting data.

TEST(DISABLED_Preparation, CollectGrammarFeatures)
{
	GrammarFeatureTrainer trainer;
	trainer.CollectFeatures("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_wo_pos.txt");
}

TEST(Preparation, ComputeWeights)
{
	GrammarFeatureTrainer trainer;
	trainer.ComputeWeights("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_wo_pos.txt");
}