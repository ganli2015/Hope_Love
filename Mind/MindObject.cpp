#include "stdafx.h"
#include "MindObject.h"
#include "DBContainer.h"

namespace Mind
{ 
	MindObject::MindObject()
	{
		_dbContainer = DBContainer::Instance();
	}


	MindObject::~MindObject()
	{
		
	}
}

