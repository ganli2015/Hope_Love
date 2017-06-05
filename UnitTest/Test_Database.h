#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

#include "../Mathmatic/Rand.h"

#include "../MindInterface/CommonFunction.h"

namespace CommonTool
{
	class DBoperator;
}

namespace DataCollection
{
	enum PartOfSpeech;
}

namespace Mind
{
	class MindDatabase;
	class MindParameterDatabase;
	class MindElementCreator;
	class ConceptDatabase;

	//////////////////////////////////////////////////////////////////////////
	//The database in <me> is a test database in "TestSample/test.db".
	//////////////////////////////////////////////////////////////////////////
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
		//Add base concept "我" to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_WO(ConceptDatabase *db);
		//////////////////////////////////////////////////////////////////////////
		//Add base concept "我要" to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_WOYAO(ConceptDatabase *db);
		//////////////////////////////////////////////////////////////////////////
		//Add base concept "我好" to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_WOHAO(ConceptDatabase *db);
		//////////////////////////////////////////////////////////////////////////
		//Add base concept "你好" to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_NIHAO(ConceptDatabase *db);
		//////////////////////////////////////////////////////////////////////////
		//Add base concept "我" with POS noun to database.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConceptToDB_WO_Noun(ConceptDatabase *db);
	};

	//////////////////////////////////////////////////////////////////////////
	//The database in <me> is a normal database in "HopeLoveData/data.db".
	//////////////////////////////////////////////////////////////////////////
	class Test_Database_Normal :public testing::Test
	{
	protected:
		static CommonTool::DBoperator *_DBOperator;

		static void SetUpTestCase();
		static void TearDownTestCase();

		static void SetDBOperator(MindDatabase* db, CommonTool::DBoperator* dbope);
		static void SetElemCreator(MindDatabase* db, MindElementCreator* creator);

		//////////////////////////////////////////////////////////////////////////
		//<meIdentity> : concept to get.
		//<toIdentity> : concept that <meIdentity> depends on.
		//<tableStr> : the concept interactive table string between <meIdentity> and  <toIdentity>.
		//////////////////////////////////////////////////////////////////////////
		static void GetNonBaseConceptWithMock(const Identity meIdentity,  const DataCollection::PartOfSpeech mePOS, 
			const Identity toIdentity, const string tableStr);
	};
}



