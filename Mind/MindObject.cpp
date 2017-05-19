#include "stdafx.h"
#include "MindObject.h"
#include "DBContainer.h"

#include "../MindElement/MindElementCreator.h"

namespace Mind
{ 
	MindObject::MindObject()
	{
		_dbContainer = DBContainer::Instance();
		_elemCreator = new MindElementCreator();
	}


	MindObject::~MindObject()
	{
		
	}
}

