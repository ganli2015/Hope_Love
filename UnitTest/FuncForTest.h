#pragma once
#include "PublicHeader.h"

namespace Mind
{
	class iConcept;
	class iConceptChain;
	class iConceptInteractTable;
}

namespace DataCollection
{
	class Word;
	enum PartOfSpeech;
}

namespace LogicSystem
{
	class iReduceResult;
}

class InitCerebrum:public testing::Test
{
	int _initObjectCount;
protected:
	virtual void SetUp();
	virtual void TearDown();
};

class AddPatternToCerebrum:public testing::Test
{
	int _initObjectCount;
protected:
	virtual void SetUp();
	virtual void TearDown();
};

class Flags
{
public:
	//A flag for Unit Test running.
	static bool UNIT_TEST;
};

#define CHECK_SKIP 	if (Flags::UNIT_TEST)\
{\
	cout<< "[  SKIPPED ] Running Unit Test.This test is skipped."<<endl;\
	SUCCEED() ;\
	return;\
}


namespace FuncForTest
{
	const string TestSampleDir="TestSample\\";
	const string dbPath = TestSampleDir + "testDB.db";
	const string LargeDataPath = "HopeLoveData\\large\\";
	const string SimpleDataPath = "HopeLoveData\\";


	shared_ptr<DataCollection::Word> ToWord(const string str, const DataCollection::PartOfSpeech pos);

	shared_ptr<Mind::iConcept> SimpleConcept(const string str);

	bool PairSameWithTable(const vector<pair<string,string>>& expect,const shared_ptr<Mind::iConceptInteractTable> table);

	bool SameTable(const shared_ptr<Mind::iConceptInteractTable> left,const shared_ptr<Mind::iConceptInteractTable> right);

	bool SameChainVec(const vector<vector<string>>& expect, const vector<shared_ptr<Mind::iConceptChain>>& result);

	bool SameChain(const vector<string>& expect,const shared_ptr<Mind::iConceptChain> res);

	void DisplayChains(const vector<shared_ptr<Mind::iConceptChain>>& chains);

	void DisplayConceptTable(const shared_ptr<Mind::iConceptInteractTable> table);

	string TablePairToString(const vector<pair<string,string>>& tablePairs);

	///Add a grammar pattern to cerebrum.
	///Pattern is Numeral,Adjective,Preposition,Numeral.
	void AddGrammarPatternToCerebrum();

	shared_ptr<Mind::iConcept> GetConcept(const string str,const int id);

	//////////////////////////////////////////////////////////////////////////
	//Parse a string to word list.
	//String should be like this: "Ȼ��/8 ��/7 ��Ƭ/0 ����/1 ��/14"
	//////////////////////////////////////////////////////////////////////////
	vector<shared_ptr<DataCollection::Word>> ParsePOSTagging(const string line);

	///Check whether there are the same elements in v1 and v2, regardless with order.
	template<class T>
	bool ContainSameElements(const vector<vector<T>>& v1,const vector<vector<T>>& v2)
	{
		if(v1.size()!=v2.size()) return false;

		vector<vector<T>> v2Copy=v2;
		for (size_t i=0;i<v1.size();++i)
		{
			bool exist=false;
			int existIndex=-1;
			for (size_t j=0;j<v2Copy.size();++j)
			{
				if(ContainSameElements(v1[i],v2Copy[j]))
				{
					exist=true;
					existIndex=j;
					break;
				}
			}

			if(!exist)
			{
				return false;
			}
			else
			{
				v2Copy.erase(v2Copy.begin()+existIndex);
			}
		}

		return true;
	}

	///Check whether v1 and v2 are the same, taking the order into account.
	template<class T>
	bool ContainSameElements(const vector<T>& v1,const vector<T>& v2)
	{
		if(v1.size()!=v2.size())
		{
			return false;
		}

		for (size_t i=0;i<v1.size();++i)
		{
			if(v1[i]!=v2[i])
			{
				return false;
			}
		}

		return true;
	}

	///Check whether the logic result <result> is the same with <tablePairs> and <conceptStr>.
	template<class ResultType>
	bool SameLogicResult(const vector<pair<string,string>>& tablePairs, const string conceptStr, const shared_ptr<ResultType> result)
	{
		if(tablePairs.empty() && conceptStr=="")
		{
			if(result==NULL)
			{
				return true;
			}
			else
				return false;
		}

		shared_ptr<iConceptInteractTable> resTable=result->GetConceptTable();
		shared_ptr<iConcept> resConcept=result->GetSingleConcept();

		//Both iConceptInteractTable and iConcept may be the result.
		if(!tablePairs.empty())
		{
			if(resTable!=NULL && FuncForTest::PairSameWithTable(tablePairs,resTable))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(resTable==NULL)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		if(conceptStr!="")
		{
			if(conceptStr==resConcept->GetString())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(resConcept==NULL)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
