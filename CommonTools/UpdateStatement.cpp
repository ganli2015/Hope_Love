#include "stdafx.h"
#include "UpdateStatement.h"
#include "QueryStatement.h"
#include "CommonStringFunction.h"


namespace CommonTool
{
	UpdateStatement::UpdateStatement(const string tableName) :_tableName(tableName),_constraint(new ConstraintStatement())
	{
	}


	UpdateStatement::~UpdateStatement()
	{
		delete _constraint;
	}

	std::string UpdateStatement::GetString() const
	{
		//Create head.
		string res = StringFormat("Update %s ", _tableName.c_str());

		vector<string> updateTerms;//All update terms.

		//Collect update terms.
		for (auto str : _stringForUpdate)
		{
			string append = StringFormat("%s='%s'", str.first.c_str(),
				CommonTool::AsciiToUtf8(str.second).c_str());
			updateTerms.push_back(append);
		}
		for (auto intPair : _intForUpdate)
		{
			string append = StringFormat("%s='%d'", intPair.first.c_str(),
				intPair.second);
			updateTerms.push_back(append);
		}
		for (auto longPair : _longForUpdate)
		{
			string append = StringFormat("%s='%ld'", longPair.first.c_str(),
				longPair.second);
			updateTerms.push_back(append);
		}

		if (updateTerms.empty())
		{
			return res;
		}
		else
		{
			res += "where ";

			//Join all updates.
			for (int i = 0; i < updateTerms.size(); ++i)
			{
				res += " " + updateTerms[i] + " ";
				if (i != updateTerms.size() - 1)
				{
					res += ",";
				}
			}

			return res;
		}
	}

	void UpdateStatement::Update(const string fieldName, const string val)
	{
		_stringForUpdate.push_back(make_pair(fieldName, val));
	}

	void UpdateStatement::Update(const string fieldName, const int val)
	{
		_intForUpdate.push_back(make_pair(fieldName, val));
	}

	void UpdateStatement::Update(const string fieldName, const long val)
	{
		_longForUpdate.push_back(make_pair(fieldName, val));
	}

}

