// UnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <exception>
#include <iostream>
using namespace std;

#include "TestClass.h"
#include "PublicHeader.h"
#include "FuncForTest.h"
#include "TestPerformance.h"
#include "UTInitializer.h"
#include "IntegrationTest.h"

#include "../Mind/FilePath.h"

#include "../CommonTools/MemoryDetector.h"
#include "../CommonTools/LogWriter.h"

class MyGlobal: public testing::Environment
{
	int _startObjCount;

public:
	~MyGlobal(){TearDown();}

	virtual void SetUp();
	virtual void TearDown();
};

void MyGlobal::SetUp()
{
	LOG("----------------------------------------Unit Test----------------------------------------");
	_startObjCount = MyObject::GetObjectCount();
}

void MyGlobal::TearDown()
{
	ASSERT_EQ(MyObject::GetObjectCount(),_startObjCount);

	LOG("/n");
	LOG("/n");
	LOG("/n");
}

void RunFilter(const string filterStr);


void RunUnitTest(int argc, _TCHAR* argv[])
{
	//Read filter information from file.
	ifstream in("config.txt");
	string line;
	getline(in, line);

	if (line != "")
	{
		RunFilter(line);
	}

#ifdef _RUN_INTEGRATION_TEST

	RunFilter("Integration*");

#endif // !_RUN_INTEGRATION_TEST

	testing::InitGoogleTest(&argc, argv);

	MEMOCHECK;
	RUN_ALL_TESTS();
}

//#define _RUN_PERFORMANCE

Initializer init;
int _cdecl _tmain(int argc, _TCHAR* argv[])
{
#ifdef _RUN_PERFORMANCE

	RunPerformance::Run();

#else

	RunUnitTest(argc, argv);

#endif // _RUN_PERFORMANCE

	system("pause");

	return 0;

}

void RunFilter(const string filterStr)
{
	::testing::GTEST_FLAG(filter) = filterStr;
}

