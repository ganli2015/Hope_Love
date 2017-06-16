#pragma once
#include "InOut.h"


namespace Mind
{
	class GrammarFeatureDatabase;
	class MindParameterDatabase;
	class ConceptDatabase;

	//////////////////////////////////////////////////////////////////////////
	//A factory containing all database object for Mind data.
	//////////////////////////////////////////////////////////////////////////
	class _MINDDATABASEINOUT DBContainer
	{
	protected:
		string _dbPath;
	public:
		DBContainer();
		virtual ~DBContainer();

		void SetDatabasePath(const string dbPath) { _dbPath = dbPath; }

		//////////////////////////////////////////////////////////////////////////
		//Get a unique_ptr for database.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<GrammarFeatureDatabase> GetGrammarFeatureDatabase() const;
		//////////////////////////////////////////////////////////////////////////
		//<grammarFeatureTypeCount> is the count of grammar feature types.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<MindParameterDatabase> GetMindParameterDatabase(const int grammarFeatureTypeCount) const;
		virtual shared_ptr<ConceptDatabase> GetConceptDatabase() const;
	private:

	};
}


