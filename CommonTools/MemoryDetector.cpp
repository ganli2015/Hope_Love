#include "stdafx.h"
#include "MemoryDetector.h"



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
		

		_s1 = _s2;
		_s2 = tmp;
	}

	bool MemoryDetector::UnchangedSnapshot() const
	{
		_CrtMemState diff;
		if (_CrtMemDifference(&diff, &_s1, &_s2))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

}

