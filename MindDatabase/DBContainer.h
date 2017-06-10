#pragma once
#include "InOut.h"


namespace Mind
{
	class GrammarFeatureDatabase;
	class MindParameterDatabase;
	class ConceptDatabase;

	//////////////////////////////////////////////////////////////////////////
	//A factory containing all database object for Mind data.
	//It is singleton.
	//////////////////////////////////////////////////////////////////////////
	class _MINDDATABASEINOUT DBContainer
	{
		static DBContainer* _instance;

		string _dbPath;
	public:

		static DBContainer* Instance();

		void SetDatabasePath(const string dbPath) { _dbPath = dbPath; }

		//////////////////////////////////////////////////////////////////////////
		//Get a unique_ptr for database.
		//////////////////////////////////////////////////////////////////////////
		unique_ptr<GrammarFeatureDatabase> GetGrammarFeatureDatabase() const;
		//////////////////////////////////////////////////////////////////////////
		//<grammarFeatureTypeCount> is the count of grammar feature types.
		//////////////////////////////////////////////////////////////////////////
		unique_ptr<MindParameterDatabase> GetMindParameterDatabase(const int grammarFeatureTypeCount) const;
		unique_ptr<ConceptDatabase> GetConceptDatabase() const;
	private:
		DBContainer();
		~DBContainer();
	};
}


