#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

#include "../Mathmatic/Rand.h"

namespace CommonTool
{
	class DBoperator;
}

namespace Mind
{
	class MindDatabase;
	class MindParameterDatabase;
	class ConceptDatabase;

	class Test_Database :public testing::Test
	{
	protected:
		static CommonTool::DBoperator *_testDBOperator;
		static Math::Rand _rand;

		static void SetUpTestCase();
		static void TearDownTestCase();

		static void SetDBOperator(MindDatabase* db, CommonTool::DBoperator* dbope);
		static void SetFeatureWeightsID(MindParameterDatabase* db, vector<string> weightID);

		//////////////////////////////////////////////////////////////////////////
		//Get random Decimal of two digits behind decimal point.
		//////////////////////////////////////////////////////////////////////////
		static double GetRandomDecimal();

		virtual void SetUp();

		//////////////////////////////////////////////////////////////////////////
		//Add base concept "Œ“" to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_WO(ConceptDatabase *db);
	};
}



