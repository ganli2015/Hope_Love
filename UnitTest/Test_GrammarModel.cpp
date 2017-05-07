#include "stdafx.h"
#include "Test_GrammarModel.h"
#include "FuncForTest.h"

#include "../Mind/GrammarLocalModel.h"
#include "../Mind/GrammarFeatureModel.h"

#include "../DataCollection/Word.h"

using namespace DataCollection;
using namespace FuncForTest;
namespace Mind
{
	TEST(Test_GrammarLocalModel, POSwithNoLocalDistribution)
	{
		//The total count of backward or forward of a POS is zero,
		//then the result of ComputePossiblity should be not NAN.

		vector<shared_ptr<Word>> wordList;
		wordList.push_back(ToWord("沙石峪", Noun));
		wordList.push_back(ToWord("那", Pronoun));
		wordList.push_back(ToWord("瘠薄", Adjective));
		wordList.push_back(ToWord("的", Auxiliary));
		wordList.push_back(ToWord("土地", Noun));
		wordList.push_back(ToWord("和", Conjunction));
		wordList.push_back(ToWord("大寨", Noun));
		wordList.push_back(ToWord("的", Auxiliary));
		wordList.push_back(ToWord("稳产高产", Noun));
		wordList.push_back(ToWord("海绵", Noun));
		wordList.push_back(ToWord("田", Noun));
		wordList.push_back(ToWord("实在", Adverb));
		wordList.push_back(ToWord("不能", Verb));
		wordList.push_back(ToWord("相比", Verb));
		wordList.push_back(ToWord("啊", Interjection));
		wordList.push_back(ToWord("！", Noun));

		GrammarLocalModel localModel;
		localModel.ReadGrammarLocal();
		auto result = localModel.ComputePossiblity(wordList);

		ASSERT_FALSE(_isnan(result));
	}

#ifndef _DEBUG


	TEST_F(Test_GrammarFeatureModel, ComputePossibility0)
	{
		auto optimal = ComputeOptimal(_samples[0]);

		ASSERT_EQ(_samples[0].expectOptimal, optimal)
			<< "The optimal sentence is " + optimal;
	}

	TEST_F(Test_GrammarFeatureModel, ComputePossibility1)
	{
		auto optimal = ComputeOptimal(_samples[1]);

		ASSERT_EQ(_samples[1].expectOptimal, optimal)
			<< "The optimal sentence is " + optimal;
	}

	TEST_F(Test_GrammarFeatureModel, ComputePossibility2)
	{
		auto optimal = ComputeOptimal(_samples[2]);

		ASSERT_EQ(_samples[2].expectOptimal, optimal)
			<< "The optimal sentence is " + optimal;
	}


	void Test_GrammarFeatureModel::SetUpTestCase()
	{
		Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
		_featureModel = new GrammarFeatureModel();
		_featureModel->LoadAllFeatures();
		PrepareSamples();
	}

	void Test_GrammarFeatureModel::TearDownTestCase()
	{
		Mind::SetHopeLoveMindPath(FuncForTest::SimpleDataPath);
		delete _featureModel;
		_samples.clear();
	}

	void Test_GrammarFeatureModel::PrepareSamples()
	{
		{
			Sample sample;
			sample.sentences.push_back("不/6 会/1 真/0 信/0 了/9 吧/11");
			sample.sentences.push_back("不/6 会/1 真/0 信/0 了/1 吧/11");
			sample.sentences.push_back("不/6 会/1 真/0 信/1 了/9 吧/11");
 			sample.sentences.push_back("不/6 会/1 真/2 信/0 了/9 吧/11");
 			sample.sentences.push_back("不/6 会/1 真/6 信/1 了/9 吧/11");
			sample.expectOptimal = "不/6 会/1 真/6 信/1 了/9 吧/11";

			_samples.push_back(sample);
		}

		{
			Sample sample;
			sample.sentences.push_back("那/5 你/5 了解/1 一下/3 吧/11");
			sample.sentences.push_back("那/8 你/5 了解/1 一下/3 吧/11");
			sample.sentences.push_back("那/5 你/5 了解/0 一下/3 吧/11");
			sample.sentences.push_back("那/0 你/5 了解/0 一下/3 吧/11");

			sample.expectOptimal = "那/8 你/5 了解/1 一下/3 吧/11";

			_samples.push_back(sample);
		}

		{
			Sample sample;
			sample.sentences.push_back("又/6 没/6 吃/1 晚饭/0 吧/11");
			sample.sentences.push_back("又/6 没/1 吃/1 晚饭/0 吧/11");

			sample.expectOptimal = "又/6 没/6 吃/1 晚饭/0 吧/11";

			_samples.push_back(sample);
		}

	}

	std::string Test_GrammarFeatureModel::ComputeOptimal(const Sample& sample)
	{
		string optimalSentence = "";
		double maxP = -1;
		for (auto sen : sample.sentences)
		{
			auto wordList = FuncForTest::ParsePOSTagging(sen);
			auto P = _featureModel->ComputePossiblity(wordList);

			if (P > maxP)
			{
				optimalSentence = sen;
				maxP = P;
			}
		}

		return optimalSentence;
	}

	vector<Test_GrammarFeatureModel::Sample> Test_GrammarFeatureModel::_samples;

	GrammarFeatureModel* Test_GrammarFeatureModel::_featureModel;

#endif // !DEBUG

}
