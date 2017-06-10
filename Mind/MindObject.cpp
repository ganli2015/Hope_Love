#include "stdafx.h"
#include "MindObject.h"

#include "../MindElement/MindElementCreator.h"

#include "../MindDatabase/DBContainer.h"

namespace Mind
{ 
	MindObject::MindObject()
	{
		_dbContainer = DBContainer::Instance();
		_dbContainer->SetDatabasePath(Mind::GetDatabasePath());
		_elemCreator = new MindElementCreator();
	}


	MindObject::~MindObject()
	{
		
	}
}

