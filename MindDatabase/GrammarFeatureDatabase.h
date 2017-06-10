#pragma once
#include "MindDatabase.h"


namespace DataCollection
{
	class GrammarFeature;
}

namespace Mind
{
	typedef vector<shared_ptr<DataCollection::GrammarFeature>> FeatureList;

	class _MINDDATABASEINOUT GrammarFeatureDatabase : public MindDatabase
	{
		const string TableName;
	public:
		GrammarFeatureDatabase(const string dbPath);
		~GrammarFeatureDatabase();


		void Insert(const shared_ptr<DataCollection::GrammarFeature> feature);
		//////////////////////////////////////////////////////////////////////////
		//Insert many features in a transaction.
		//////////////////////////////////////////////////////////////////////////
		void Insert(const vector<shared_ptr<DataCollection::GrammarFeature>> features);

		//////////////////////////////////////////////////////////////////////////
		//Number of rows.
		//////////////////////////////////////////////////////////////////////////
		long RowCount();


		//////////////////////////////////////////////////////////////////////////
		//Get all features in database.
		//////////////////////////////////////////////////////////////////////////
		FeatureList GetAllFeatures();

	private:

		

		CommonTool::DBCmd CreateCommand(const string statement) const;
	};

	
}


