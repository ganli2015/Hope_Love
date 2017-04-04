#include "stdafx.h"
#include "TestAlgorithm.h"
#include "TestPOSTagging.h"

#include "../Mind/Cerebrum.h"

using namespace Mind;

TEST_F(TestAlgorithm, TestPOSTagging)
{
	TestPOSTagging test;
	test.Run("E:\\Artificial Intelligence\\Project\\Hope_Love\\DataProcessor\\Corpus\\New\\corpus_de_sub.txt", 100);
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
