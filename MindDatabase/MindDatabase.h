#pragma once
#include "InOut.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
	class DBRow;
	class QueryStatement;
	class UpdateStatement;
}


namespace Mind
{
	class MindElementCreator;

	//////////////////////////////////////////////////////////////////////////
	//The base class of Database classes for Mind.
	//////////////////////////////////////////////////////////////////////////
	class _MINDDATABASEINOUT MindDatabase
	{
		string _dbPath;

		friend class Test_Database;
		friend class Test_Database_Normal;
	protected:
		CommonTool::DBoperator *_db;
		MindElementCreator *_elemCreator;
	public:
		MindDatabase(const string dbPath);
		~MindDatabase();

		void Connect();
		void Disconnect();

	protected:
		//////////////////////////////////////////////////////////////////////////
		//Check if database is connected.
		//If not, then connect.
		//////////////////////////////////////////////////////////////////////////
		void CheckConnect();

		vector<CommonTool::DBRow> QueryRows(const CommonTool::QueryStatement& cmd);
		vector<CommonTool::DBRow> QueryRows(const string& cmd);

		void UpdateDatabase(const CommonTool::UpdateStatement& statement);

		//////////////////////////////////////////////////////////////////////////
		//Get row from primary key in the table.
		//////////////////////////////////////////////////////////////////////////
		CommonTool::DBRow GetRow(const string pk, const string pkColName, const string table);
		bool HasRow(const string pk, const string pkColName, const string table);
	};
}


