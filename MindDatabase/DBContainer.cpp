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
	
	shared_ptr<GrammarFeatureDatabase> DBContainer::GetGrammarFeatureDatabase() const
	{
		shared_ptr<GrammarFeatureDatabase> db(new GrammarFeatureDatabase(_dbPath));
		db->Connect();

		return db;
	}

	shared_ptr<MindParameterDatabase> DBContainer::GetMindParameterDatabase(const int grammarFeatureTypeCount) const
	{
		shared_ptr<MindParameterDatabase> db(new MindParameterDatabase(_dbPath, grammarFeatureTypeCount));
		db->Connect();

		return db;
	}

	shared_ptr<ConceptDatabase> DBContainer::GetConceptDatabase() const
	{
		shared_ptr<ConceptDatabase> db(new ConceptDatabase(_dbPath));
		db->Connect();

		return db;
	}

}

