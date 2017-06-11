#include "stdafx.h"
#include "MindObject.h"

#include "ConceptReadWriter.h"

#include "../CommonTools/Common.h"

#include "../MindElement/MindElementCreator.h"


namespace Mind
{ 
	MindObject::MindObject()
	{
		_dbContainer = new CachedDBContainer(Mind::GetDatabasePath());
		_elemCreator = new MindElementCreator();
	}


	MindObject::~MindObject()
	{
		CommonTool::TryDeletePointer(_elemCreator);
		CommonTool::TryDeletePointer(_dbContainer);
	}

	CachedDBContainer::CachedDBContainer(const string dbPath):_conceptReadWriter(NULL)
	{
		if (_conceptReadWriter == NULL)
		{
			_conceptReadWriter = new ConceptReadWriter(dbPath);
			_dbPath = dbPath;
		}
	}

	CachedDBContainer::~CachedDBContainer()
	{
		CommonTool::TryDeletePointer(_conceptReadWriter);
	}

	unique_ptr<ConceptDatabase> CachedDBContainer::GetConceptDatabase() const
	{
		_conceptReadWriter->Initialize();
		unique_ptr<ConceptDatabase> db(_conceptReadWriter);
		db->Connect();

		return db;
	}

}

