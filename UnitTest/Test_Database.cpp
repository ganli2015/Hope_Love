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

#include "../UTFacility/MockConcept.h"
#include "../UTFacility/MockMindElementCreator.h"
#include "../UTFacility/ConceptTableCreator.h"

#include <gmock/gmock.h>
#include <gmock/gmock-spec-builders.h>

using namespace DataCollection;
using ::testing::StrictMock;
using ::testing::NiceMock;

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

		auto getConcept = db->GetBaseConcept(index);

		ASSERT_EQ(index, getConcept->GetBaseId());
		ASSERT_EQ(id, getConcept->GetId());
		ASSERT_EQ(wordStr, getConcept->GetString());
		ASSERT_EQ(pos, getConcept->GetPartOfSpeech());
	}

	TEST_F(Test_Database, ReadWriteNonBaseConcept)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);

		auto word= LanguageFunc::GetParticularWord("你", Pronoun);

		db->AddNonBaseConcept(word);
		auto concept = db->GetNonBaseConcept(0, "你");

		ASSERT_EQ("你", concept->GetString());
		ASSERT_EQ(Pronoun, concept->GetPartOfSpeech());

		try
		{
			auto concept = db->GetNonBaseConcept(1, "你");
		}
		catch (const std::exception&)
		{
			//Get exception when the id is invalid.
			SUCCEED();
		}
	}

	TEST_F(Test_Database, ReadWriteNonBaseConcept_IDAutomaticallyIncrease)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);

		auto word = LanguageFunc::GetParticularWord("你", Pronoun);

		db->AddNonBaseConcept(word);
		db->AddNonBaseConcept(word);//Same word.Id will be increased by one.
		auto concept = db->GetNonBaseConcept(1, "你");

		//Concept with id 1 can be read from database.
		ASSERT_TRUE(concept != NULL);
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

	TEST_F(Test_Database, HasString_ConceptDatabase)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);
		AddBaseConceptToDB_WO(db);

		ASSERT_TRUE(db->HasString("我"));
	}

	TEST_F(Test_Database, GetConceptsWithHead)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);
		AddBaseConceptToDB_WO(db);
		AddBaseConceptToDB_WOYAO(db);
		AddBaseConceptToDB_WOHAO(db);
		AddBaseConceptToDB_NIHAO(db);

		auto chara = make_shared<Character>("我");

		auto concepts = db->GetConceptsWithHead(chara);

		ASSERT_EQ(3, concepts.size());
		//Check all start with "我".
		for (auto concept : concepts)
		{
			auto startChara = concept->GetWord()->GetFirstCharacter();
			ASSERT_TRUE(startChara.IsSame(*chara));
		}
	}

	TEST_F(Test_Database, GetConceptsWithWord)
	{
		ConceptDatabase *db = new ConceptDatabase();
		SetDBOperator(db, _testDBOperator);
		AddBaseConceptToDB_WO(db);//Pronoun 
		AddBaseConceptToDB_WO_Noun(db);//Noun
		AddBaseConceptToDB_NIHAO(db);

		auto concepts = db->GetConceptsWithWord("我");

		ASSERT_EQ(2, concepts.size());

		auto word1 = concepts[0]->GetWord();
		ASSERT_EQ("我", word1->GetString());
		ASSERT_EQ(Pronoun, word1->Type());

		auto word2 = concepts[1]->GetWord();
		ASSERT_EQ("我", word2->GetString());
		ASSERT_EQ(Noun, word2->Type());
	}

	TEST_F(Test_Database_Normal, GetNonBaseConcept_WithNoMod)
	{
		//Get Non base concept with no modification.
		//Expect <AddForwardConcept> is called.
		//Test word is “我”.

		Identity meIdentity("我", 0);
		Identity toIdentity("自己", 0);
		string modStr = "";

		GetNonBaseConceptWithMock(meIdentity, Pronoun, toIdentity, modStr);
	}

	TEST_F(Test_Database_Normal, GetNonBaseConcept_WithOneSingleMod)
	{
		//Get Non base concept with one forward concept edge.
		//And the forward edge has one single modification.
		//Test word is “喜欢”.

		Identity meIdentity("喜欢", 0);
		Identity toIdentity("好感", 0);
		string modStr = "大-好感";

		GetNonBaseConceptWithMock(meIdentity, Verb, toIdentity, modStr);
	}

	TEST_F(Test_Database_Normal, GetNonBaseConcept_WithTwoSingleMod)
	{
		//Get Non base concept with one forward concept edge.
		//And the forward edge has two single modifications.
		//Test word is “爱”.The connection is "0 爱 to 0 喜欢 0 深深 0 非常"
		//which has two modifications.

		Identity meIdentity("爱", 0);
		Identity toIdentity("喜欢", 0);
		string modStr = "深深-喜欢,非常-喜欢";

		GetNonBaseConceptWithMock(meIdentity, Verb, toIdentity, modStr);
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
		_testDBOperator->DeleteRowsInTable("NonBaseConcept");
	}

	void Test_Database::AddBaseConceptToDB_WO(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我", Pronoun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(0);
		concept->SetId(0);

		db->AddBaseConcept(concept);
	}

	void Test_Database::AddBaseConceptToDB_WOYAO(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我要", Pronoun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(1);
		concept->SetId(0);

		db->AddBaseConcept(concept);
	}

	void Test_Database::AddBaseConceptToDB_WOHAO(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我好", Pronoun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(2);
		concept->SetId(0);

		db->AddBaseConcept(concept);
	}

	void Test_Database::AddBaseConceptToDB_NIHAO(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("你好", Pronoun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(3);
		concept->SetId(0);

		db->AddBaseConcept(concept);
	}

	void Test_Database::AddBaseConceptToDB_WO_Noun(ConceptDatabase *db)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord("我", Noun);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetBaseId(4);
		concept->SetId(1);

		db->AddBaseConcept(concept);
	}

	Math::Rand Test_Database::_rand;

	CommonTool::DBoperator * Test_Database::_testDBOperator;

	void Test_Database_Normal::SetUpTestCase()
	{
		_DBOperator = new CommonTool::DBoperator(Mind::GetDatabasePath());
	}

	void Test_Database_Normal::TearDownTestCase()
	{
		delete _DBOperator;
	}

	void Test_Database_Normal::SetDBOperator(MindDatabase* db, CommonTool::DBoperator* dbope)
	{
		db->_db = dbope;
	}

	void Test_Database_Normal::SetElemCreator(MindDatabase* db, MindElementCreator* creator)
	{
		db->_elemCreator = creator;
	}

	void Test_Database_Normal::GetNonBaseConceptWithMock(const Identity meIdentity, const PartOfSpeech mePOS,
		const Identity toIdentity, 
		const string tableStr)
	{
		shared_ptr<Word> word = LanguageFunc::GetParticularWord(meIdentity.str, mePOS);
		//Prepare mock objects.
		shared_ptr<StrictMock<MockConcept>> mockConcept(new StrictMock<MockConcept>(word));
		//Add expect for to_concept.
		mockConcept->AddExpectCall_AddForwardConcept(toIdentity, 1);

		if (tableStr != "")
		{
			//Add expect for modification table.
			ConceptTableCreator tableCreator;
			auto modTable = tableCreator.SimpleCreate(tableStr);
			mockConcept->AddExpectCall_AddForwardModification(toIdentity, modTable, 1);
		}

		MockMindElementCreator* mockElemCreator = new MockMindElementCreator;
		//Add condition for creating mock concept.
		CommonTool::DBRow meRow;
		meRow.Insert("word", meIdentity.str);
		meRow.Insert("id", meIdentity.id);
		mockElemCreator->SetCreatedConcept(meRow, mockConcept);

		//Inject mock object.
		StrictMock<ConceptDatabase> *db = new StrictMock<ConceptDatabase>();
		SetDBOperator(db, _DBOperator);
		SetElemCreator(db, mockElemCreator);

		ASSERT_NO_FATAL_FAILURE(db->GetNonBaseConcept(meIdentity.id, meIdentity.str));

		ASSERT_TRUE(testing::Mock::VerifyAndClearExpectations(mockConcept.get()));
		ASSERT_TRUE(testing::Mock::VerifyAndClearExpectations(db));
	}

	CommonTool::DBoperator * Test_Database_Normal::_DBOperator=NULL;

}

