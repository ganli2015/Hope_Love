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
		//CheckHasTable();
	}


	GrammarFeatureDatabase::~GrammarFeatureDatabase()
	{
		if (_db != NULL)
		{
			_db->Disconnect();
			delete _db;
			_db = NULL;
		}

	}

	void GrammarFeatureDatabase::Insert(const shared_ptr<DataCollection::GrammarFeature> feature)
	{
		CheckConnect();

		auto cmd = feature->GetInsertCmd(*_db);
		cmd.Execute();
	}

	long GrammarFeatureDatabase::RowCount()
	{
		CheckConnect();

		string state = "select * from " + TableName;
		DBQry qry(state, *_db);
		//Compute count;
		return qry.RowCount();
	}

	void GrammarFeatureDatabase::Disconnect()
	{
		if(_db!=NULL)
			_db->Disconnect();
	}

	void GrammarFeatureDatabase::CheckConnect()
	{
		if (_db == NULL)
		{
			_db = new DBoperator(GetDatabasePath());
		}
	}

	DBCmd Mind::GrammarFeatureDatabase::CreateCommand(const string statement) const
	{
		DBCmd cmd(statement,*_db);
		return cmd;
	}

	

}

