#include "stdafx.h"
#include "DBContainer.h"

#include "GrammarFeatureDatabase.h"
#include "MindParameterDatabase.h"
#include "ConceptDatabase.h"

namespace Mind
{
	DBContainer::DBContainer()
	{
	}


	DBContainer::~DBContainer()
	{
	}
	
	unique_ptr<GrammarFeatureDatabase> DBContainer::GetGrammarFeatureDatabase() const
	{
		unique_ptr<GrammarFeatureDatabase> db(new GrammarFeatureDatabase(_dbPath));
		db->Connect();

		return db;
	}

	unique_ptr<MindParameterDatabase> DBContainer::GetMindParameterDatabase(const int grammarFeatureTypeCount) const
	{
		unique_ptr<MindParameterDatabase> db(new MindParameterDatabase(_dbPath, grammarFeatureTypeCount));
		db->Connect();

		return db;
	}

	unique_ptr<ConceptDatabase> DBContainer::GetConceptDatabase() const
	{
		unique_ptr<ConceptDatabase> db(new ConceptDatabase(_dbPath));
		db->Connect();

		return db;
	}

}

