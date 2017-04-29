#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

namespace CommonTool
{
	class DBoperator;
}

namespace Mind
{
	class MindDatabase;
	class MindParameterDatabase;

	class Test_Database :public testing::Test
	{
	protected:
		static CommonTool::DBoperator *_testDBOperator;

		static void SetUpTestCase();
		static void TearDownTestCase();

		static void SetDBOperator(MindDatabase* db, CommonTool::DBoperator* dbope);
		static void SetFeatureWeightsID(MindParameterDatabase* db, vector<string> weightID);
	};
}



