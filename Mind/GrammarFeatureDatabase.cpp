#include "stdafx.h"
#include "GrammarFeatureDatabase.h"

#include "../CommonTools/LogWriter.h"
#include "../CommonTools/DBoperator.h"

#include "../DataCollection/GrammarFeature.h"

#include "FilePath.h"




using namespace CommonTool;

namespace Mind
{
	GrammarFeatureDatabase::GrammarFeatureDatabase() :TableName("GrammarFeature"), _db(NULL)
	{
		CheckHasTable();
	}


	GrammarFeatureDatabase::~GrammarFeatureDatabase()
	{
		_db->Disconnect();
		delete _db;
	}

	void GrammarFeatureDatabase::Insert(const shared_ptr<GrammarFeature> feature)
	{
		CheckConnect();

	}

	long GrammarFeatureDatabase::RowCount()
	{
		CheckConnect();

		string state = "select * from " + TableName;
		DBQry qry(state, *_db);
		//Compute count;
		return qry.RowCount();
	}

	void GrammarFeatureDatabase::CheckHasTable()
	{
		CheckConnect();
		//if (RowCount() != 0) return;

		DEBUG_FORMAT("Not find table %s.", TableName.c_str());
		string state = "CREATE TABLE " + TableName + "\
				(ID int(8) PRIMARY KEY  NOT NULL,\
				Freq int(8)  NOT NULL,\
				Type char(15) NOT NULL,\
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
		DBCmd cmd = CreateCommand(state);
		cmd.Execute();
	}

	void GrammarFeatureDatabase::CheckConnect()
	{
		if (_db == NULL)
		{
			_db = new DBoperator();
		}

		_db->Connect(GetDatabasePath());
	}

	DBCmd Mind::GrammarFeatureDatabase::CreateCommand(const string statement) const
	{
		DBCmd cmd(statement,*_db);
		return cmd;
	}

	

}

