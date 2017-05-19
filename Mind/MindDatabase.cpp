#include "stdafx.h"
#include "MindDatabase.h"

#include "../CommonTools/DBoperator.h"

#include "../MindElement/MindElementCreator.h"

using namespace CommonTool;

namespace Mind
{
	MindDatabase::MindDatabase():_db(NULL)
	{
		_elemCreator = new MindElementCreator();
	}


	MindDatabase::~MindDatabase()
	{
		if (_db != NULL)
		{
			_db->Disconnect();
			delete _db;
			_db = NULL;
		}
	}

	void MindDatabase::Connect()
	{
		CheckConnect();
	}

	void MindDatabase::Disconnect()
	{
		if (_db != NULL)
			_db->Disconnect();
	}

	void MindDatabase::CheckConnect()
	{
		if (_db == NULL)
		{
			_db = new DBoperator(GetDatabasePath());
		}
	}
}

