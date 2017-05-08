#include "stdafx.h"
#include "DBContainer.h"

#include "GrammarFeatureDatabase.h"
#include "MindParameterDatabase.h"
#include "ConceptDatabase.h"

namespace Mind
{

	DBContainer* DBContainer::_instance = NULL;

	DBContainer* DBContainer::Instance()
	{
		if (_instance == NULL)
		{
			_instance = new DBContainer();
		}

		return _instance;
	}

	unique_ptr<GrammarFeatureDatabase> DBContainer::GetGrammarFeatureDatabase() const
	{
		unique_ptr<GrammarFeatureDatabase> db(new GrammarFeatureDatabase());
		db->Connect();

		return db;
	}

	unique_ptr<MindParameterDatabase> DBContainer::GetMindParameterDatabase() const
	{
		unique_ptr<MindParameterDatabase> db(new MindParameterDatabase());
		db->Connect();

		return db;
	}

	unique_ptr<ConceptDatabase> DBContainer::GetConceptDatabase() const
	{
		unique_ptr<ConceptDatabase> db(new ConceptDatabase());
		db->Connect();

		return db;
	}

	DBContainer::DBContainer()
	{
	}


	DBContainer::~DBContainer()
	{
	}
}

