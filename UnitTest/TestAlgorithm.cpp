#include "stdafx.h"
#include "TestAlgorithm.h"
#include "TestPOSTagging.h"

#include "../Mind/Cerebrum.h"
#include "../Mind/GrammarFeatureModel.h"

#include "../CommonTools/CommonStringFunction.h"

using namespace Mind;
using namespace CommonTool;

TEST_F(TestAlgorithm, TestPOSTagging)
{
	TestPOSTagging test;
	test.Run("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_mine.txt");
}

TEST_F(TestAlgorithm, TestPOSTagging_OneSentence)
{
	string unSplitSentence = "然后/8 将/7 照片/0 贴上/1 ，/14";
	auto expect = FuncForTest::ParsePOSTagging(unSplitSentence);

	string unSplitSentence2 = "然后/8 将/1 照片/0 贴/1 上/1 ，/14";
	auto result = FuncForTest::ParsePOSTagging(unSplitSentence2);

	GrammarFeatureModel model;
	model.LoadAllFeatures();
	auto expectP = model.ComputePossiblity(expect);
	auto resultP= model.ComputePossiblity(result);

	cout << "Expect P is " + ToString(expectP) << endl;
	cout << "Result P is " + ToString(resultP) << endl;
}

void TestAlgorithm::SetUp()
{
	Mind::SetHopeLoveMindPath("HopeLoveData\\large\\");
	iCerebrum::SetInstance(Cerebrum::Instance());
}

void TestAlgorithm::TearDown()
{
	Mind::SetHopeLoveMindPath("HopeLoveData\\");
	iCerebrum::KillInstance();
}
