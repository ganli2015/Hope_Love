#include "stdafx.h"
#include "DBoperator.h"
#include "CommonStringFunction.h"

#include <sqlite3pp/sqlite3pp.h>

using namespace sqlite3pp;

namespace CommonTool
{
	DBoperator::DBoperator(const string dbPath)
	{
		_db = new database(dbPath.c_str());
	}


	DBoperator::~DBoperator()
	{
		Disconnect();
	}

	void DBoperator::Disconnect()
	{
		if (_db != NULL)
		{
			_db->disconnect();
			_db = NULL;
		}
	}

	void DBoperator::DeleteTable(const string tableName)
	{
		DBCmd cmd("Drop table " + tableName, *this);
		cmd.Execute();
	}

	DBCmd::DBCmd(const string cmd, DBoperator db)
	{
		_cmd = new command(*db._db, cmd.c_str());
	}

	DBCmd::~DBCmd()
	{
		if(_cmd!=NULL)
			delete _cmd;
	}

	void DBCmd::Execute()
	{
		_cmd->execute();
	}

	void DBCmd::Bind(const string key, const long val)
	{
		_cmd->bind(key.c_str(), val);
	}

	void DBCmd::Bind(const string key, const string val)
	{
		_cmd->bind(key.c_str(), val, sqlite3pp::copy);
	}

	void DBCmd::Bind(const int index, const long val)
	{
		_cmd->bind(index+1, val);
	}

	void DBCmd::Bind(const int index, const string val)
	{
		_cmd->bind(index+1, val, sqlite3pp::copy);
	}

	DBQry::DBQry(const string cmd, DBoperator db)
	{
		_qry = new query(*db._db, cmd.c_str());

		//Add data to rows.
		for (sqlite3pp::query::iterator i = _qry->begin(); i != _qry->end(); ++i) 
		{
			auto row = (*i);
			DBRow dbRow;
			for (int j = 0; j < _qry->column_count(); ++j) 
			{
				if (row.column_type(j) == SQLITE3_TEXT)
				{
					dbRow.Insert(_qry->column_name(j), row.get<const char*>(j));
				}
				else if (row.column_type(j) == SQLITE_INTEGER)
				{
					dbRow.Insert(_qry->column_name(j), row.get<const long>(j));
				}
			}

			_rows.push_back(dbRow);
		}
	}

	DBQry::~DBQry()
	{
		if (_qry != NULL)
			delete _qry;
	}

	long DBQry::RowCount() const
	{
		return _rows.size();
	}

	DBRow::DBRow()
	{
	}

	DBRow::~DBRow()
	{
	}

	void DBRow::Insert(const string colName, const long val)
	{
		_longType[colName]=val;
	}

	void DBRow::Insert(const string colName, const string val)
	{
		_strType[colName] = val;
	}

	long DBRow::GetLong(const string colName) const
	{
		if (_longType.find(colName) != _longType.end())
		{
			return _longType.at(colName);
		}

		throw invalid_argument("Invalid column name: " + colName);
	}

	string DBRow::GetText(const string colName) const
	{
		if (_strType.find(colName) != _strType.end())
		{
			return _strType.at(colName);
		}

		throw invalid_argument("Invalid column name: " + colName);
	}
}

