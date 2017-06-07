#include "stdafx.h"
#include "MyException.h"

#include "CommonStringFunction.h"

namespace CommonTool
{
	IterationDiverge::IterationDiverge():runtime_error("")
	{
	}

	IterationDiverge::~IterationDiverge()
	{
	}

	DatabaseException::DatabaseException():DatabaseException("")
	{

	}

	DatabaseException::DatabaseException(string tableName, string msg ):_tableName(tableName),_msg(msg)
	{

	}

	DatabaseException::DatabaseException(const DatabaseException& rhs)
	{
		this->_tableName = rhs._tableName;
	}

	char const* DatabaseException::what() const
	{
		return StringFormat("Database error.Table name: %s. Error message: %s.",_tableName.c_str(),_msg.c_str());
	}

}
