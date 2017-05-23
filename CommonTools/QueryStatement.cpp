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

		vector<string> conditions;//All constraint terms.

		//Like constraint.
		for (auto like : _likeConditions)
		{
			string append = StringFormat("%s LIKE '%s'", like.first,
				CommonTool::AsciiToUtf8(like.second).c_str());
			conditions.push_back(append);
		}

		//Collect equality conditions.
		for (auto strEq : _eqConditionsString)
		{
			string append = StringFormat("%s='%s'", strEq.first.c_str(),
				CommonTool::AsciiToUtf8(strEq.second).c_str());
			conditions.push_back(append);
		}
		for (auto intEq : _eqConditionsInt)
		{
			string append = StringFormat("%s='%d'", intEq.first.c_str(),
				intEq.second);
			conditions.push_back(append);
		}
		for (auto longEq : _eqConditionsLong)
		{
			string append = StringFormat("%s='%ld'", longEq.first.c_str(),
				longEq.second);
			conditions.push_back(append);
		}

		if (conditions.empty())
		{
			return res;
		}
		else
		{
			res += "where ";

			//Join like constraints.
			for (int i = 0; i < conditions.size(); ++i)
			{
				res += " " + conditions[i] + " ";
				if (i != conditions.size() - 1)
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

	void QueryStatement::Like(const string fieldName, const string format)
	{
		_likeConditions.push_back(make_pair(fieldName, format));
	}

}

