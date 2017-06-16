#include "stdafx.h"
#include "MindObject.h"

#include "ConceptReadWriter.h"

#include "../CommonTools/Common.h"
#include "../CommonTools/ConfigureInfoManager.h"
#include "../CommonTools/CommonStringFunction.h"

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
			_conceptReadWriter = make_shared<ConceptReadWriter>(dbPath);
			_dbPath = dbPath;
		}
	}

	CachedDBContainer::~CachedDBContainer()
	{
// 		if (_conceptReadWriter.use_count() == 1)
// 		{
// 			//It is only used by me and if i am released then no one use it.
// 			_conceptReadWriter.~shared_ptr();
// 		}
	}

	shared_ptr<ConceptDatabase> CachedDBContainer::GetConceptDatabase() const
	{
		//Try to get Monitor interval in the config file.
		string intervalStr = CommonTool::ConfigureInfoManager::GetInstance()->GetValue("CACHE_MONITOR_INTERVAL");
		if (intervalStr != "")
		{
			int interval = CommonTool::StringToNum<int>(intervalStr);
			_conceptReadWriter->SetCacheMonitorInterval(interval);
		}

		_conceptReadWriter->Initialize();
		_conceptReadWriter->Connect();

		return _conceptReadWriter;
	}

}

