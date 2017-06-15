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
		wordList.push_back(ToWord("ɳʯ��", Noun));
		wordList.push_back(ToWord("��", Pronoun));
		wordList.push_back(ToWord("񤱡", Adjective));
		wordList.push_back(ToWord("��", Auxiliary));
		wordList.push_back(ToWord("����", Noun));
		wordList.push_back(ToWord("��", Conjunction));
		wordList.push_back(ToWord("��կ", Noun));
		wordList.push_back(ToWord("��", Auxiliary));
		wordList.push_back(ToWord("�Ȳ��߲�", Noun));
		wordList.push_back(ToWord("����", Noun));
		wordList.push_back(ToWord("��", Noun));
		wordList.push_back(ToWord("ʵ��", Adverb));
		wordList.push_back(ToWord("����", Verb));
		wordList.push_back(ToWord("���", Verb));
		wordList.push_back(ToWord("��", Interjection));
		wordList.push_back(ToWord("��", Noun));

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
			sample.sentences.push_back("��/6 ��/1 ��/0 ��/0 ��/9 ��/11");
			sample.sentences.push_back("��/6 ��/1 ��/0 ��/0 ��/1 ��/11");
			sample.sentences.push_back("��/6 ��/1 ��/0 ��/1 ��/9 ��/11");
 			sample.sentences.push_back("��/6 ��/1 ��/2 ��/0 ��/9 ��/11");
 			sample.sentences.push_back("��/6 ��/1 ��/6 ��/1 ��/9 ��/11");
			sample.expectOptimal = "��/6 ��/1 ��/6 ��/1 ��/9 ��/11";

			_samples.push_back(sample);
		}

		{
			Sample sample;
			sample.sentences.push_back("��/5 ��/5 �˽�/1 һ��/3 ��/11");
			sample.sentences.push_back("��/8 ��/5 �˽�/1 һ��/3 ��/11");
			sample.sentences.push_back("��/5 ��/5 �˽�/0 һ��/3 ��/11");
			sample.sentences.push_back("��/0 ��/5 �˽�/0 һ��/3 ��/11");

			sample.expectOptimal = "��/8 ��/5 �˽�/1 һ��/3 ��/11";

			_samples.push_back(sample);
		}

		{
			Sample sample;
			sample.sentences.push_back("��/6 û/6 ��/1 ��/0 ��/11");
			sample.sentences.push_back("��/6 û/1 ��/1 ��/0 ��/11");

			sample.expectOptimal = "��/6 û/6 ��/1 ��/0 ��/11";

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
