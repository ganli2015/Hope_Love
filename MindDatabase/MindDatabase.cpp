#include "stdafx.h"
#include "MindDatabase.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/QueryStatement.h"
#include "../CommonTools/UpdateStatement.h"
#include "../CommonTools/DeleteStatement.h"

#include "../MindElement/MindElementCreator.h"

using namespace CommonTool;

namespace Mind
{
	MindDatabase::MindDatabase(const string dbPath):_db(NULL),_dbPath(dbPath)
	{
		_elemCreator = new MindElementCreator();
	}


	MindDatabase::~MindDatabase()
	{
		if (_db != NULL)
		{
			_db->Disconnect();
			delete _db;
			_db = NULL;
		}
	}

	void MindDatabase::Connect()
	{
		CheckConnect();
	}

	void MindDatabase::Disconnect()
	{
		if (_db != NULL)
			_db->Disconnect();
	}

	void MindDatabase::BeginTransaction()
	{
		_db->BeginTransaction();
	}

	void MindDatabase::CommitTransaction()
	{
		_db->CommitTransaction();
	}

	void MindDatabase::CheckConnect()
	{
		if (_db == NULL)
		{
			_db = new DBoperator(_dbPath);
		}
	}

	vector<DBRow> MindDatabase::QueryRows(const CommonTool::QueryStatement& state)
	{
		return QueryRows(state.GetString());
	}

	void MindDatabase::UpdateDatabase(const CommonTool::UpdateStatement& statement)
	{
		DBCmd cmd(statement.GetString(), *_db);
		cmd.Execute();
	}

	void MindDatabase::DeleteRow(const CommonTool::DeleteStatement& statement)
	{
		DBCmd cmd(statement.GetString(), *_db);
		cmd.Execute();
	}

	vector<CommonTool::DBRow> MindDatabase::QueryRows(const string& cmd)
	{
		CheckConnect();

		DBQry qry(cmd, *_db);
		auto rows = qry.GetRows();
		return rows;
	}

	CommonTool::DBRow MindDatabase::GetRow(const string pk, const string pkColName, const string table)
	{
		QueryStatement qryState(table);
		qryState.EQ(pkColName, pk);
		DBQry qry(qryState.GetString(), *_db);
		auto rows = qry.GetRows();
		//There must be only one row.
		return rows.front();
	}

	bool MindDatabase::HasRow(const string pk, const string pkColName, const string table)
	{
		QueryStatement qryState(table);
		qryState.EQ(pkColName, pk);
		DBQry qry(qryState.GetString(), *_db);
		auto rows = qry.GetRows();

		return !rows.empty();
	}
}

