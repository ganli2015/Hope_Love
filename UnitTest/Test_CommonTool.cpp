#include "StdAfx.h"
#include "Test_CommonTool.h"

#include "../Mind/Cerebrum.h"
#include "../Mind/ConceptSet.h"
#include "../Mind/GrammaSet.h"

#include "../LogicSystem/LogicKnowledge.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/MemoryDetector.h"

#include <Windows.h>

using namespace Mind;
using namespace LogicSystem;
using namespace CommonTool;

TEST(MemoryLeak,ConceptSet)
{
	int objCountStart=MyObject::GetObjectCount();
	ConceptSet* cs=new ConceptSet();
	int objCountInit=MyObject::GetObjectCount();
	delete cs;
	int objCountEnd=MyObject::GetObjectCount();

	ASSERT_EQ(objCountStart,objCountEnd);
}

TEST(MemoryLeak,GrammarSet)
{
	int objCountStart=MyObject::GetObjectCount();
	GrammarSet* cs=new GrammarSet();
	int objCountInit=MyObject::GetObjectCount();
	delete cs;
	int objCountEnd=MyObject::GetObjectCount();

	ASSERT_EQ(objCountStart,objCountEnd);
}
TEST(MemoryLeak,Cerebrum)
{
	int objCountStart=MyObject::GetObjectCount();
	iCerebrum::SetInstance(Cerebrum::Instance());
	int objCountInit=MyObject::GetObjectCount();
	iCerebrum::KillInstance();
	int objCountEnd=MyObject::GetObjectCount();


	ASSERT_EQ(objCountStart,objCountEnd);
}

TEST(MemoryLeak,LogicKnowledge)
{
	int objCountStart=MyObject::GetObjectCount();
	LogicKnowledge* knowledge=new LogicKnowledge();
	int objCountInit=MyObject::GetObjectCount();
	delete knowledge;
	int objCountEnd=MyObject::GetObjectCount();


	ASSERT_EQ(objCountStart,objCountEnd);
}

TEST(Test_CommonStringFunction,TrimBeginEndBlank)
{
	string origin="  °¡°¡°¡ ";
	CommonTool::TrimBeginEndBlank(origin);

	ASSERT_EQ("°¡°¡°¡",origin);
}

TEST(Test_CommonStringFunction, StringFormat)
{
	char* format = "%d %s %lf";
	string res = StringFormat(format, 1, "hello", 2.5);

	string expect = "1 hello 2.500000";

	ASSERT_EQ(expect, res);
}

TEST(Test_CommonStringFunction, GenerateID)
{
	string str1 = "0-Äã";
	string str2 = "1-Äã";
	string str3 = "0-ÎÒ";

	string id1 = GenerateID(str1);
	string id2 = GenerateID(str2);
	string id3 = GenerateID(str3);

	ASSERT_NE(id1, id2);
	ASSERT_NE(id2, id3);
}