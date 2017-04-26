#pragma once
#include "InOut.h"


namespace Mind
{
	class GrammarFeatureDatabase;
	class MindParameterDatabase;

	//////////////////////////////////////////////////////////////////////////
	//A factory containing all database object for Mind data.
	//It is singleton.
	//////////////////////////////////////////////////////////////////////////
	class DBContainer
	{
		static DBContainer* _instance;

	public:

		static DBContainer* Instance();

		//////////////////////////////////////////////////////////////////////////
		//Get a unique_ptr for database.
		//////////////////////////////////////////////////////////////////////////
		unique_ptr<GrammarFeatureDatabase> GetGrammarFeatureDatabase() const;
		unique_ptr<MindParameterDatabase> GetMindParameterDatabase() const;
	private:
		DBContainer();
		~DBContainer();
	};
}


