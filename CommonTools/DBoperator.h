#pragma once
#include "InOut.h"

namespace sqlite3pp {
	class database;
	class command;
	class query;
}

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

namespace CommonTool
{
	//////////////////////////////////////////////////////////////////////////
	//Database operator.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT DBoperator
	{
		sqlite3pp::database *_db;

		friend class DBCmd;
		friend class DBQry;
	public:
		DBoperator(const string dbPath);
		~DBoperator();

		void Disconnect();

		void DeleteTable(const string tableName);
	};

	class _COMMONTOOLSINOUT DBCmd
	{
		sqlite3pp::command *_cmd;
	public:
		DBCmd(const string cmd,DBoperator db);
		~DBCmd();

		void Execute();

		//////////////////////////////////////////////////////////////////////////
		//Bind the value with key in the initial command string.
		//////////////////////////////////////////////////////////////////////////
		void Bind(const string key, const long val);
		void Bind(const string key, const string val);
		//////////////////////////////////////////////////////////////////////////
		//Bind the value with the parameter index in the initial command string.(start from 0)
		//////////////////////////////////////////////////////////////////////////
		void Bind(const int index, const long val);
		void Bind(const int index, const string val);
	};

	//////////////////////////////////////////////////////////////////////////
	//A row in a table.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT DBRow
	{
		map<string, long> _longType;
		map<string, string> _strType;

		friend class DBQry;
	public:
		template<class T>
		struct ReturnTrait
		{
			typedef typename T type;
		};

	public:
		DBRow();
		~DBRow();


		long GetLong(const string colName) const;
		string GetText(const string colName) const;
	private:

		void Insert(const string colName, const long val);
		void Insert(const string colName, const string val);
	};

	class _COMMONTOOLSINOUT DBQry
	{
		sqlite3pp::query *_qry;

		vector<DBRow> _rows;

	public:
		DBQry(const string cmd, DBoperator db);
		~DBQry();

		long RowCount() const;

		vector<DBRow> GetRows() const { return _rows; }

	private:

	};



}


