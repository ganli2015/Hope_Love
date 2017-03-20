#include "stdafx.h"
#include "MemoryDetector.h"



namespace CommonTool
{
	MemoryDetector::MemoryDetector()
	{
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
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

	std::string MemoryDetector::PrintDiff() const
	{
		char buffer[1000];
		sprintf_s(buffer, "Last second statistics:{%d,%d,%d,%d,%d}.Last statistics:{%d,%d,%d,%d,%d}. ",
			_s1.lCounts[0], _s1.lCounts[1], _s1.lCounts[2], _s1.lCounts[3], _s1.lCounts[4],
			_s2.lCounts[0], _s2.lCounts[1], _s2.lCounts[2], _s2.lCounts[3], _s2.lCounts[4]);

		return buffer;
	}

	void MemoryDetector::OutputLatestStat() const
	{
		_CrtMemDumpStatistics(&_s2);
	}

}

