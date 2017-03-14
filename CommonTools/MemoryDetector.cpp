#include "stdafx.h"
#include "MemoryDetector.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

namespace CommonTool
{
	MemoryDetector::MemoryDetector()
	{
	}


	MemoryDetector::~MemoryDetector()
	{
	}

	void MemoryDetector::Snapshot()
	{
		_CrtMemState tmp;
		_CrtMemCheckpoint(&tmp);
		_stateVec.push_back(tmp);
	}

	bool MemoryDetector::UnchangedSnapshot() const
	{
		if (_stateVec.size() < 2)
		{
			return false;
		}
		else
		{
			auto lastState = _stateVec.back();
			auto lastSecondState = _stateVec[_stateVec.size() - 2];
			_CrtMemState diff;
			if (_CrtMemDifference(&diff, &lastState, &lastSecondState))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

}

