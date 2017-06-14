#include "stdafx.h"
#include "TestAlgorithm.h"
#include "TestPOSTagging.h"
#include "FuncForTest.h"

#include "../Mind/Cerebrum.h"
#include "../Mind/GrammarFeatureModel.h"

#include "../CommonTools/CommonStringFunction.h"

using namespace Mind;
using namespace CommonTool;


TEST_F(TestAlgorithm, TestPOSTagging)
{
	CHECK_SKIP;

	TestPOSTagging test;
	//test.Run("D:\\Projects\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_mine.txt");
	test.Run("E:\\Artificial Intelligence\\Document\\DataBase\\myconv_train.txt",20);
}

TEST_F(TestAlgorithm, TestPOSTagging_OneSentence)
{
// 	string unSplitSentence = "然后/8 将/7 照片/0 贴上/1 ，/14";
// 	auto expect = FuncForTest::ParsePOSTagging(unSplitSentence);
// 
// 	string unSplitSentence2 = "然后/8 将/1 照片/0 贴/1 上/1 ，/14";
// 	auto result = FuncForTest::ParsePOSTagging(unSplitSentence2);
// 
// 	GrammarFeatureModel model;
// 	model.LoadAllFeatures();
// 	auto expectP = model.ComputePossiblity(expect);
// 	auto resultP= model.ComputePossiblity(result);
// 
// 	cout << "Expect P is " + ToString(expectP) << endl;
// 	cout << "Result P is " + ToString(resultP) << endl;
	CHECK_SKIP;

	TestPOSTagging test;
	test.POSTagging("不会真信了吧！");
}

void TestAlgorithm::SetUp()
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	iCerebrum::SetInstance(Cerebrum::Instance());
}

void TestAlgorithm::TearDown()
{
	CHECK_SKIP;
	Mind::SetHopeLoveMindPath(FuncForTest::SimpleDataPath);
	iCerebrum::KillInstance();
}

