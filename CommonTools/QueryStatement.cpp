#include "stdafx.h"
#include "QueryStatement.h"

#include "CommonStringFunction.h"

namespace CommonTool
{
	QueryStatement::QueryStatement(const string tableName) :_tableName(tableName)
	{
	}


	QueryStatement::~QueryStatement()
	{
	}

	string QueryStatement::GetString() const
	{
		//Create head.
		string res = StringFormat("Select * from %s ", _tableName.c_str());
		//Collect equality conditions.
		vector<string> eqConditions;
		for (auto strEq : _eqConditionsString)
		{
			string append = StringFormat("%s='%s'", strEq.first.c_str(),
				CommonTool::AsciiToUtf8(strEq.second).c_str());
			eqConditions.push_back(append);
		}
		for (auto intEq : _eqConditionsInt)
		{
			string append = StringFormat("%s='%d'", intEq.first.c_str(),
				intEq.second);
			eqConditions.push_back(append);
		}
		for (auto longEq : _eqConditionsLong)
		{
			string append = StringFormat("%s='%ld'", longEq.first.c_str(),
				longEq.second);
			eqConditions.push_back(append);
		}

		if (eqConditions.empty())
		{
			return res;
		}
		else
		{
			res += "where ";
			for (int i = 0; i < eqConditions.size(); ++i)
			{
				res += " " + eqConditions[i] + " ";
				if (i != eqConditions.size() - 1)
				{
					res += "and";
				}
			}

			return res;
		}
	}

	void QueryStatement::EQ(const string fieldName, const string val)
	{
		_eqConditionsString.push_back(make_pair(fieldName, val));
	}

	void QueryStatement::EQ(const string fieldName, const int val)
	{
		_eqConditionsInt.push_back(make_pair(fieldName, val));
	}

	void QueryStatement::EQ(const string fieldName, const long val)
	{
		_eqConditionsLong.push_back(make_pair(fieldName, val));
	}

}

