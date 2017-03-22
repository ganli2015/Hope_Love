#include "stdafx.h"
#include "Test_GrammarFeature.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/GrammarFeature.h"
#include "../DataCollection/GrammarFeatureTemplate.h"
#include "../DataCollection/LanguageFunc.h"

using namespace FuncForTest;

namespace DataCollection
{
	INSTANTIATE_TEST_CASE_P(Test_GrammarFeature, Test_FindFeatures, testing::ValuesIn(Test_FindFeatures::GenerateSamples()));

	TEST_P(Test_FindFeatures, Sentence1)
	{
		//Sentence: "听说年纪越大越容易。"
		vector<shared_ptr<Word>> sentence;
		sentence.push_back(ToWord("听说", Verb));
		sentence.push_back(ToWord("年纪", Noun));
		sentence.push_back(ToWord("越", Adverb));
		sentence.push_back(ToWord("大", Adjective));
		sentence.push_back(ToWord("越", Adverb));
		sentence.push_back(ToWord("容易", Adjective));
		sentence.push_back(ToWord("。", Punctuation));

		auto param= GetParam();
		//Find features.
		auto features = param.featureTemplate->FindFeatures(sentence);
		ASSERT_EQ(param.featureCount, features.size());
	}

	INSTANTIATE_TEST_CASE_P(Test_GrammarFeature, Test_FeatureCount, testing::ValuesIn(Test_FeatureCount::GenerateSamples()));

	TEST_P(Test_FeatureCount, Sentence1)
	{
		//Sentence: "听说年纪越大越容易。"
		vector<shared_ptr<Word>> sentence;
		sentence.push_back(ToWord("听说", Verb));
		sentence.push_back(ToWord("年纪", Noun));
		sentence.push_back(ToWord("越", Adverb));
		sentence.push_back(ToWord("大", Adjective));
		sentence.push_back(ToWord("越", Adverb));
		sentence.push_back(ToWord("容易", Adjective));
		sentence.push_back(ToWord("。", Punctuation));

		auto param = GetParam();
		//Find features.
		auto featureCount = param.feature->FeatureCount(sentence);
		ASSERT_EQ(param.featureCount, featureCount);
	}

	vector<Param_FindFeatures> Test_FindFeatures::_res;

	vector<Param_FindFeatures> Test_FindFeatures::GenerateSamples()
	{
		PushSample<TagWithWordTemplate>(7);
		PushSample<TagBigramTemplate>(6);
		PushSample<TagTrigramTemplate>(5);
		PushSample<TagFollowedByWordTemplate>(6);
		PushSample<WordFollowedByTagTemplate>(6);

		return _res;
	}

	vector<Param_FeatureCount> Test_FeatureCount::_res;

	vector<Param_FeatureCount> Test_FeatureCount::GenerateSamples()
	{
		auto tagWithWord = make_shared<TagWithWord>(ToWord("越", Adverb));
		PushSample(tagWithWord, 2);

		auto tagBigram = make_shared<TagBigram>(Adverb,Adjective);
		PushSample(tagBigram, 2);

		auto tagTrigram = make_shared<TagTrigram>(Adverb, Adjective,Adverb);
		PushSample(tagTrigram, 1);

		auto tagFollowedByWord = make_shared<TagFollowedByWord>(Adverb,ToWord("大",Adjective));
		PushSample(tagFollowedByWord, 1);

		auto wordFollowedByTag = make_shared<WordFollowedByTag>(ToWord("越", Adverb), Adjective);
		PushSample(wordFollowedByTag, 2);

		return _res;
	}

}

