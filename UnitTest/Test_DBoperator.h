#pragma once
#include "PublicHeader.h"
#include "FuncForTest.h"

#include "../CommonTools/DBoperator.h"

struct TestRow
{
	long id;
	string name1;
	string name2;
	long long1;
	long long2;
};

class Test_DBoperator
{
public:
	Test_DBoperator();
	~Test_DBoperator();

	static TestRow GetRowData(const CommonTool::DBRow row);

};

Test_DBoperator::Test_DBoperator()
{
}

Test_DBoperator::~Test_DBoperator()
{
}

inline TestRow Test_DBoperator::GetRowData(const CommonTool::DBRow row)
{
	TestRow res;
	res.id = row.GetLong("id");
	res.name1 = row.GetText("name1");
	res.name2 = row.GetText("name2");
	res.long1 = row.GetLong("long1");
	res.long2 = row.GetLong("long2");

	return res;
}



