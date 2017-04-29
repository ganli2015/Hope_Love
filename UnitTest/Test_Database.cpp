#include "stdafx.h"
#include "Test_Database.h"

#include "../Mind/MindParameterDatabase.h"
#include "../Mind/GrammarFeatureDatabase.h"

#include "../CommonTools/DBoperator.h"

namespace Mind
{
	TEST_F(Test_Database, ReadWriteGrammarFeatureModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = 0.1;
		db->WriteGrammarFeatureModelWeight(weight);

		auto getVal = db->GetGrammarFeatureModelWeight();

		ASSERT_EQ(weight, getVal);
	}

	TEST_F(Test_Database, ReadWriteGrammarLocalModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = 0.2;
		db->WriteGrammarLocalModelWeight(weight);

		auto getVal = db->GetGrammarLocalModelWeight();

		ASSERT_EQ(weight, getVal);
	}

	TEST_F(Test_Database, ReadWriteGrammarPatternModelWeight)
	{
		MindParameterDatabase *db = new MindParameterDatabase();
		SetDBOperator(db, _testDBOperator);

		double weight = 0.3;
		db->WriteGrammarPatternModelWeight(weight);

		auto getVal = db->GetGrammarPatternModelWeight();

		ASSERT_EQ(weight, getVal);
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

		vector<double> weights{ 0.1,0.2,0.3,0.4,0.5 };
		db->UpdateGrammarFeatureWeights(weights);

		auto getVal = db->GetGrammarFeatureWeights();

		ASSERT_EQ(weights.size(), getVal.size());
		for (unsigned i=0;i<weights.size();++i)
		{
			ASSERT_EQ(weights[i], getVal[i]);
		}
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

	CommonTool::DBoperator * Test_Database::_testDBOperator;

}

