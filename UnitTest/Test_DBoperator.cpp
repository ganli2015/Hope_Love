#include "stdafx.h"
#include "Test_DBoperator.h"

#include "../Mind/FilePath.h"
#include <sqlite3pp/sqlite3pp.h>

const string tableName = "t1";

using namespace CommonTool;
using namespace FuncForTest;

TEST(Test_DBoperator, QueryRowCount)
{
	DBoperator dbOpe(dbPath);
	DBQry qry("select * from " + tableName, dbOpe);

	ASSERT_EQ(4, qry.RowCount());
}

TEST(Test_DBoperator, GetRow1)
{
	DBoperator dbOpe(dbPath);
	DBQry qry("select * from " + tableName, dbOpe);
	auto rows = qry.GetRows();

	auto row1 = rows[0];
	TestRow row = Test_DBoperator::GetRowData(row1);

	ASSERT_EQ(1, row.id);
	ASSERT_EQ("aa", row.name1);
	ASSERT_EQ("aa2", row.name2);
	ASSERT_EQ(10000, row.long1);
	ASSERT_EQ(1, row.long2);
}

TEST(Test_DBoperator, GetRow2)
{
	DBoperator dbOpe(dbPath);
	DBQry qry("select * from " + tableName, dbOpe);
	auto rows = qry.GetRows();

	auto row1 = rows[1];
	TestRow row = Test_DBoperator::GetRowData(row1);

	ASSERT_EQ(2, row.id);
	ASSERT_EQ("bb", row.name1);
	ASSERT_EQ("", row.name2);
	ASSERT_EQ(20000, row.long1);
	ASSERT_EQ(2, row.long2);
}

TEST(Test_DBoperator, EmptyLongInRow)
{
	//A field of long is empty. Once we read it, we catch exception.
	DBoperator dbOpe(dbPath);
	DBQry qry("select * from " + tableName, dbOpe);
	auto rows = qry.GetRows();

	auto row1 = rows[3];
	try
	{
		TestRow row = Test_DBoperator::GetRowData(row1);
	}
	catch (const std::exception&)
	{
		ASSERT_TRUE(true);
	}
}

TEST(Test_DBoperator, InsertRow)
{
	//A field of long is empty. Once we read it, we catch exception.
	DBoperator dbOpe(dbPath);
	//Create table.
	DBCmd cmd("Create table t2(id Integer primary key,long1 Integer,name1 Char(10))", dbOpe);
	cmd.Execute();

	//Insert rows.
	
	//Bind with column name.
	string state = "INSERT INTO t2 (long1, name1) VALUES (:long1, :name1)";
	DBCmd insertCmd(state, dbOpe);
	insertCmd.Bind(":long1", 1);
	insertCmd.Bind(":name1", "aa");
	/*insertCmd.Bind(1, 1);
	insertCmd.Bind(2, "aa");*/
	insertCmd.Execute();

	//Bind with index.
	string state2 = "INSERT INTO t2 (long1, name1) VALUES (?, ?)";
	DBCmd insertCmd2(state2, dbOpe);
	insertCmd2.Bind(0, 2);
	insertCmd2.Bind(1, "bb");
	insertCmd2.Execute();

	//Check row value.
	DBQry qry("select * from t2", dbOpe);
	auto rows = qry.GetRows();
	ASSERT_EQ(2, qry.RowCount());

	auto row1 = rows[0];
	ASSERT_EQ(1, row1.GetLong("id"));
	ASSERT_EQ(1, row1.GetLong("long1"));
	ASSERT_EQ("aa", row1.GetText("name1"));

	auto row2 = rows[1];
	ASSERT_EQ(2, row2.GetLong("id"));
	ASSERT_EQ(2, row2.GetLong("long1"));
	ASSERT_EQ("bb", row2.GetText("name1"));

	dbOpe.DeleteTable("t2");
}

TEST(Test_DBoperator, ttt)
{
	DBoperator dbOpe(Mind::GetHopeLoveMindPath()+"data.db");
	DBQry qry("select * from BaseConceptsString" , dbOpe);

	auto rows = qry.GetRows();

// 	wstring ww = Utf8ToUnicode(rows[0].GetText("word"));
// 	string wwtf = AsciiToUtf8(rows[0].GetText("word"));
// 	string wwas = Utf8ToAscii(rows[0].GetText("word"));
	//ASSERT_EQ(4, qry.RowCount());
}
