#include "stdafx.h"
#include "DeleteStatement.h"
#include "QueryStatement.h"

#include "CommonStringFunction.h"

namespace CommonTool
{
	DeleteStatement::DeleteStatement(const string tableName) :_tableName(tableName), _constraint(new ConstraintStatement())
	{
	}


	DeleteStatement::DeleteStatement(const DeleteStatement& other)
	{
		this->_tableName = other._tableName;
		this->_constraint = new ConstraintStatement(*other._constraint);
	}

	DeleteStatement::~DeleteStatement()
	{
		delete _constraint;
	}

	string DeleteStatement::GetString() const
	{
		//Create head.
		string res = StringFormat("Delete from %s ", _tableName.c_str());

		res += _constraint->GetString();

		return res;
	}

	void DeleteStatement::EQ(const string fieldName, const string val)
	{
		_constraint->EQ(fieldName, val);
	}

	void DeleteStatement::EQ(const string fieldName, const int val)
	{
		_constraint->EQ(fieldName, val);
	}

	void DeleteStatement::EQ(const string fieldName, const long val)
	{
		_constraint->EQ(fieldName, val);
	}

	void DeleteStatement::Like(const string fieldName, const string format)
	{
		_constraint->Like(fieldName, format);
	}
}