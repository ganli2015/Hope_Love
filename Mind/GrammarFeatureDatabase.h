#pragma once
#include "InOut.h"



namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}

namespace Mind
{
	class GrammarFeature;

	class GrammarFeatureDatabase
	{
		const string TableName;
		CommonTool::DBoperator *_db;
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

		CommonTool::DBCmd CreateCommand(const string statement) const;
	};

	
}

