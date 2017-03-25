#pragma once
#include "InOut.h"

#include "../CommonTools/DatabaseTrait.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/GrammarFeature.h"

#include "FilePath.h"

namespace sqlite3pp
{
	class database;
	class command;
}

namespace Mind
{
	class GrammarFeature;

	template<class DBHelper>
	class GrammarFeatureDatabase
	{
		typedef typename Trait<DBHelper>::DB DB;
		typedef typename Trait<DBHelper>::DBCmd DBCmd;
		typedef typename Trait<DBHelper>::DBQry DBQry;

		const string TableName;
		DB *_db;
	public:
		GrammarFeatureDatabase();
		~GrammarFeatureDatabase();


		void Insert(const shared_ptr<GrammarFeature> feature);

		//////////////////////////////////////////////////////////////////////////
		//Number of rows.
		//////////////////////////////////////////////////////////////////////////
		long RowCount();

	private:

		//////////////////////////////////////////////////////////////////////////
		//Check if there is feature table.
		//If not, create one.
		//////////////////////////////////////////////////////////////////////////
		void CheckHasTable();

		//////////////////////////////////////////////////////////////////////////
		//Check if database is connected.
		//If not, then connect.
		//////////////////////////////////////////////////////////////////////////
		void CheckConnect();

		typename GrammarFeatureDatabase<DBHelper>::DBCmd* CreateCommand(const string statement) const;
	};

	template<class DBHelper>
	GrammarFeatureDatabase<DBHelper>::GrammarFeatureDatabase() :TableName("GrammarFeature"), _db(NULL)
	{
		CheckHasTable();
	}


	template<class DBHelper>
	GrammarFeatureDatabase<DBHelper>::~GrammarFeatureDatabase()
	{
	}

	template<class DBHelper>
	void GrammarFeatureDatabase<DBHelper>::Insert(const shared_ptr<GrammarFeature> feature)
	{

	}

	template<class DBHelper>
	long GrammarFeatureDatabase<DBHelper>::RowCount()
	{
		CheckConnect();

		string state = "select * from " + TableName;
		DBQry qry(*_db, state.c_str());
		//Compute count;
		long count = 0;
		for (DBQry::iterator i = qry.begin(); i != qry.end(); ++i) {
			++count;
		}

		return count;
	}

	template<class DBHelper>
	void GrammarFeatureDatabase<DBHelper>::CheckHasTable()
	{
		CheckConnect();
		//if (RowCount() != 0) return;

		DEBUG_FORMAT("Not find table %s.", TableName.c_str());
		string state = "CREATE TABLE " + TableName + "\
				(ID int(8) PRIMARY KEY  NOT NULL,\
				Freq int(8)  NOT NULL,\
				POS1 int(2),\
				POS2 int(2),\
				POS3 int(2),\
				POS4 int(2),\
				POS5 int(2),\
				Word1 int(8),\
				Word2 int(8),\
				Word3 int(8),\
				Word4 int(8),\
				Word5 int(8));";
		DBCmd* cmd = CreateCommand(state);
		cmd->execute();
		delete cmd;
	}

	template<class DBHelper>
	void GrammarFeatureDatabase<DBHelper>::CheckConnect()
	{
		if (_db == NULL)
		{
			_db = new DB(GetDatabasePath().c_str());
		}
	}

	template<class DBHelper>
	typename GrammarFeatureDatabase<DBHelper>::DBCmd* Mind::GrammarFeatureDatabase<DBHelper>::CreateCommand(const string statement) const
	{
		DBCmd* cmd = new DBCmd(*_db, statement.c_str());
		return cmd;
	}
}


