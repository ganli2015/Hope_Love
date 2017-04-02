#include "stdafx.h"
#include "GrammarFeatureDatabase.h"

#include "../CommonTools/LogWriter.h"
#include "../CommonTools/DBoperator.h"

#include "../DataCollection/GrammarFeature.h"

#include "FilePath.h"


using namespace CommonTool;

namespace Mind
{
	GrammarFeatureDatabase::GrammarFeatureDatabase() :TableName("GrammarFeature")
	{
		//CheckHasTable();
	}


	GrammarFeatureDatabase::~GrammarFeatureDatabase()
	{
	

	}

	void GrammarFeatureDatabase::Insert(const shared_ptr<DataCollection::GrammarFeature> feature)
	{
		CheckConnect();

		auto cmd = feature->GetInsertCmd(*_db);
		cmd.Execute();
	}

	void GrammarFeatureDatabase::Insert(const vector<shared_ptr<DataCollection::GrammarFeature>> features)
	{
		CheckConnect();
		_db->BeginTransaction();
		for (auto feature : features)
		{
			Insert(feature);
		}
		_db->CommitTransaction();
	}

	long GrammarFeatureDatabase::RowCount()
	{
		CheckConnect();

		string state = "select * from " + TableName;
		DBQry qry(state, *_db);
		//Compute count;
		return qry.RowCount();
	}


	Mind::FeatureList GrammarFeatureDatabase::GetAllFeatures()
	{
		CheckConnect();

		_db->BeginTransaction();
		//Get all rows.
		string state = "select * from " + TableName;
		DBQry qry(state, *_db);
		auto allRows = qry.GetRows();
		_db->CommitTransaction();

		//Transform row to feature.
		FeatureList res;
		for (auto row : allRows)
		{
			auto feature = DataCollection::GrammarFeature::GetFeature(row);
			res.push_back(feature);
		}

		return res;
	}


	DBCmd Mind::GrammarFeatureDatabase::CreateCommand(const string statement) const
	{
		DBCmd cmd(statement,*_db);
		return cmd;
	}

	

}

