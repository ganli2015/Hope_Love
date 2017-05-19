#pragma once
#include "InOut.h"

namespace Mind
{
	class DBContainer;
	class MindElementCreator;

	//////////////////////////////////////////////////////////////////////////
	//The base for Mind classes used for accessing the same object.
	//////////////////////////////////////////////////////////////////////////
	class MindObject
	{
	protected:
		DBContainer* _dbContainer;
		MindElementCreator* _elemCreator;
	public:
		MindObject();
		~MindObject();
	};
}


