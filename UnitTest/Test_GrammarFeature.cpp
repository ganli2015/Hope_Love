#include "stdafx.h"
#include "Test_GrammarFeature.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/GrammarFeature.h"
#include "../DataCollection/GrammarFeatureTemplate.h"
#include "../DataCollection/LanguageFunc.h"

#include "../CommonTools/DBoperator.h"

using namespace FuncForTest;
using namespace CommonTool;

namespace DataCollection
{
	INSTANTIATE_TEST_CASE_P(Test_GrammarFeature, Test_FindFeatures, testing::ValuesIn(Test_FindFeatures::GenerateSamples()));

	TEST_P(Test_FindFeatures, Sentence1)
	{
		//Sentence: "��˵���Խ��Խ���ס�"
		vector<shared_ptr<Word>> sentence;
		sentence.push_back(ToWord("��˵", Verb));
		sentence.push_back(ToWord("���", Noun));
		sentence.push_back(ToWord("Խ", Adverb));
		sentence.push_back(ToWord("��", Adjective));
		sentence.push_back(ToWord("Խ", Adverb));
		sentence.push_back(ToWord("����", Adjective));
		sentence.push_back(ToWord("��", Punctuation));

		auto param= GetParam();
		//Find features.
		auto features = param.featureTemplate->FindFeatures(sentence);
		ASSERT_EQ(param.featureCount, features.size());
	}

	INSTANTIATE_TEST_CASE_P(Test_GrammarFeature, Test_FeatureCount, testing::ValuesIn(Test_FeatureCount::GenerateSamples()));

	TEST_P(Test_FeatureCount, Sentence1)
	{
		//Sentence: "��˵���Խ��Խ���ס�"
		vector<shared_ptr<Word>> sentence;
		sentence.push_back(ToWord("��˵", Verb));
		sentence.push_back(ToWord("���", Noun));
		sentence.push_back(ToWord("Խ", Adverb));
		sentence.push_back(ToWord("��", Adjective));
		sentence.push_back(ToWord("Խ", Adverb));
		sentence.push_back(ToWord("����", Adjective));
		sentence.push_back(ToWord("��", Punctuation));

		auto param = GetParam();
		//Find features.
		auto featureCount = param.feature->FeatureCount(sentence);
		ASSERT_EQ(param.featureCount, featureCount);
	}

	vector<Param_FindFeatures> Test_FindFeatures::_res;

	vector<Param_FindFeatures> Test_FindFeatures::GenerateSamples()
	{
		//The number is expected feature count.
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
		auto tagWithWord = make_shared<TagWithWord>(ToWord("Խ", Adverb));
		PushSample(tagWithWord, 2);

		auto tagBigram = make_shared<TagBigram>(Adverb,Adjective);
		PushSample(tagBigram, 2);

		auto tagTrigram = make_shared<TagTrigram>(Adverb, Adjective,Adverb);
		PushSample(tagTrigram, 1);

		auto tagFollowedByWord = make_shared<TagFollowedByWord>(Adverb,"��");
		PushSample(tagFollowedByWord, 1);

		auto wordFollowedByTag = make_shared<WordFollowedByTag>("Խ", Adjective);
		PushSample(wordFollowedByTag, 2);

		auto wordTagPreChar = make_shared<WordTagPreChar>("Խ", Adverb, "��");
		PushSample(wordTagPreChar, 1);

		auto wordTagNextChar = make_shared<WordTagNextChar>("���", Noun, "Խ");
		PushSample(wordTagNextChar, 1);

		auto singleCharWithTrigramChar = make_shared<SingleCharWithTrigramChar>("��", "Խ",  "Խ",Adjective);
		PushSample(singleCharWithTrigramChar, 1);

		auto wordStartWithChar = make_shared<WordStartWithChar>("��",Adjective);
		PushSample(wordStartWithChar, 1);

		auto wordEndWithChar = make_shared<WordEndWithChar>("��", Adjective);
		PushSample(wordEndWithChar, 1);

		return _res;
	}

	INSTANTIATE_TEST_CASE_P(Test_GrammarFeature, Test_ReadWriteDB, testing::ValuesIn(Test_ReadWriteDB::GenerateSamples()));

	TEST_P(Test_ReadWriteDB, WriteToDBAndRead)
	{
		//Clear all rows in table "GrammarFeature".
		ClearFeatureRows();

		auto param = GetParam();
		//Insert feature to database.
		DBoperator db(dbPath);
		DBCmd insertCmd = param.input->GetInsertCmd(db);
		insertCmd.Execute();

		//Get row from database.
		DBQry qry("Select * from " + featureTable, db);
		auto rows = qry.GetRows();
		ASSERT_EQ(1, rows.size());

		//Check the feature is the same as input.
		auto feature = GrammarFeature::GetFeature(rows[0]);
		ASSERT_TRUE(feature->Same(param.input));
	}

	std::string Test_ReadWriteDB::featureTable = "GrammarFeature";

	vector<Param_ReadWriteDB> Test_ReadWriteDB::_params;

	vector<Param_ReadWriteDB> Test_ReadWriteDB::GenerateSamples()
	{
		//Generate samples of basic features.
		{
			auto word = ToWord("��", Pronoun);
			shared_ptr<TagWithWord> feature(new TagWithWord(word));
			PushSample(feature);
		}

		{
			shared_ptr<TagBigram> feature(new TagBigram(Noun,Verb));
			PushSample(feature);
		}

		{
			shared_ptr<TagTrigram> feature(new TagTrigram(Noun, Verb,Adjective));
			PushSample(feature);
		}

		{
			shared_ptr<TagFollowedByWord> feature(new TagFollowedByWord(Noun, "ĪĪ"));
			PushSample(feature);
		}

		{
			shared_ptr<WordFollowedByTag> feature(new WordFollowedByTag( "ĪĪ", Noun));
			PushSample(feature);
		}

		return _params;
	}

	void Test_ReadWriteDB::ClearFeatureRows()
	{
		DBoperator db(FuncForTest::dbPath);
		DBCmd cmd("Delete from " + featureTable, db);
		cmd.Execute();
	}

}

