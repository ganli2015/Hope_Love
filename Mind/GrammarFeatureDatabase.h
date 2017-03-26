#pragma once
#include "InOut.h"



namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}

namespace DataCollection
{
	class GrammarFeature;
}

namespace Mind
{

	class GrammarFeatureDatabase
	{
		const string TableName;
		CommonTool::DBoperator *_db;
	public:
		GrammarFeatureDatabase();
		~GrammarFeatureDatabase();


		void Insert(const shared_ptr<DataCollection::GrammarFeature> feature);

		//////////////////////////////////////////////////////////////////////////
		//Number of rows.
		//////////////////////////////////////////////////////////////////////////
		long RowCount();

		void Disconnect();

	private:

		//////////////////////////////////////////////////////////////////////////
		//Check if database is connected.
		//If not, then connect.
		//////////////////////////////////////////////////////////////////////////
		void CheckConnect();

		CommonTool::DBCmd CreateCommand(const string statement) const;
	};

	
}


