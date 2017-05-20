#include "stdafx.h"
#include "Test_Database.h"

#include "../Mind/MindParameterDatabase.h"
#include "../Mind/GrammarFeatureDatabase.h"
#include "../Mind/ConceptDatabase.h"

#include "../MindElement/BaseConcept.h"

#include "../CommonTools/DBoperator.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

#include "../Mathmatic/MathTool.h"

using namespace DataCollection;

namespace Mind
{
	TEST_F(Test_Database, ReadWriteGrammarFeatureModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = GetRandomDecimal();
		db->WriteGrammarFeatureModelWeight(weight);

		auto getVal = db->GetGrammarFeatureModelWeight();

		ASSERT_DOUBLE_EQ(weight, getVal);
	}

	TEST_F(Test_Database, ReadWriteGrammarLocalModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = GetRandomDecimal();
		db->WriteGrammarLocalModelWeight(weight);

		auto getVal = db->GetGrammarLocalModelWeight();

		ASSERT_DOUBLE_EQ(weight, getVal);
	}

	TEST_F(Test_Database, ReadWriteGrammarPatternModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = GetRandomDecimal();
		db->WriteGrammarPatternModelWeight(weight);

		auto getVal = db->GetGrammarPatternModelWeight();

		ASSERT_DOUBLE_EQ(weight, getVal);
	}

	TEST_F(Test_Database, ReadWriteGrammarFeatureWeights)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		//Set feature type in MindParameterDatabase.
		//The count of type determines the count of weights.
		vector<string> weightID;
		for (unsigned i = 0; i < 5; ++i)
		{
			//Get id of the weight.
			char buffer[10];
			sprintf_s(buffer, "fw%d", i);
			string id(buffer);

			weightID.push_back(id);
		}
		SetFeatureWeightsID(db, weightID);

		vector<double> weights ;
		for (int i = 0; i < 5; ++i)
		{
			weights.push_back(GetRandomDecimal());
		}
		db->UpdateGrammarFeatureWeights(weights);

		auto getVal = db->GetGrammarFeatureWeights();

		ASSERT_EQ(weights.size(), getVal.size());
		for (unsigned i=0;i<weights.size();++i)
		{
			ASSERT_DOUBLE_EQ(weights[i], getVal[i]);
		}
	}

	TEST_F(Test_Database, ReadWriteBaseConcept)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);

		long index = 0;
		int id = 2;
		string wordStr = "我";
		PartOfSpeech pos = Pronoun;

		//Create concept.
		shared_ptr<Word> word = LanguageFunc::GetParticularWord(wordStr, pos);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(index);
		concept->SetId(id);

		db->AddBaseConcept(concept);

		auto getConcept = db->ReadBaseConcept(index);

		ASSERT_EQ(index, getConcept->GetBaseId());
		ASSERT_EQ(id, getConcept->GetId());
		ASSERT_EQ(wordStr, getConcept->GetString());
		ASSERT_EQ(pos, getConcept->GetPartOfSpeech());
	}

	TEST_F(Test_Database, GetBaseConceptCount)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);

		//Add base concept to database.
		AddBaseConceptToDB_WO(db);

		auto count = db->GetBaseConceptCount();

		ASSERT_EQ(1, count);
	}

	TEST_F(Test_Database, HasWord_ConceptDatabase)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);
		AddBaseConceptToDB_WO(db);

		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我", Pronoun);
		ASSERT_TRUE(db->HasWord(word));
	}

	TEST_F(Test_Database, NotHasWord_ConceptDatabase)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);
		AddBaseConceptToDB_WO(db);

		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我", Noun);
		ASSERT_FALSE(db->HasWord(word));
	}

	void Test_Database::SetUpTestCase()
	{
		//Create a DBOperator to point to test.db.
		_testDBOperator = new CommonTool::DBoperator(FuncForTest::dbPath);
	}

	void Test_Database::TearDownTestCase()
	{
		delete _testDBOperator;
	}

	void Test_Database::SetDBOperator(MindDatabase* db, CommonTool::DBoperator* dbope)
	{
		db->_db = dbope;
	}

	void Test_Database::SetFeatureWeightsID(MindParameterDatabase* db, vector<string> weightID)
	{
		db->_featureWeightID = weightID;
	}

	double Test_Database::GetRandomDecimal()
	{
		auto rand = _rand.GetRandDouble(0, 1);
		return (int)(rand * 100) / 100.0;
	}

	void Test_Database::SetUp()
	{
		_testDBOperator->DeleteRowsInTable("BaseConceptsString");
	}

	void Test_Database::AddBaseConceptToDB_WO(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我", Pronoun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(0);
		concept->SetId(0);

		db->AddBaseConcept(concept);
	}

	Math::Rand Test_Database::_rand;

	CommonTool::DBoperator * Test_Database::_testDBOperator;

}

