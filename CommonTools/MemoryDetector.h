#pragma once
#include "InOut.h"

struct _CrtMemState;

namespace CommonTool
{
	class _COMMONTOOLSINOUT MemoryDetector
	{
		vector<_CrtMemState> _stateVec;

	public:
		MemoryDetector();
		~MemoryDetector();

		//////////////////////////////////////////////////////////////////////////
		//Record current memory status.
		//////////////////////////////////////////////////////////////////////////
		void Snapshot();

		//////////////////////////////////////////////////////////////////////////
		//If the latest two snapshot are different, return false.
		//////////////////////////////////////////////////////////////////////////
		bool UnchangedSnapshot() const;
	};
}


