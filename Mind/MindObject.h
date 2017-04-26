#pragma once
#include "InOut.h"

namespace Mind
{
	class DBContainer;

	//////////////////////////////////////////////////////////////////////////
	//The base for Mind classes used for accessing the same object.
	//////////////////////////////////////////////////////////////////////////
	class MindObject
	{
	protected:
		DBContainer* _dbContainer;
	public:
		MindObject();
		~MindObject();
	};
}


