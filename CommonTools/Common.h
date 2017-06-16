#pragma once
#include "InOut.h"

namespace CommonTool
{
	void _COMMONTOOLSINOUT SleepForSeconds(const int seconds);

	template<class T>
	void TryDeletePointer(T *ptr)
	{
		if (ptr != NULL)
		{
			delete ptr;
			ptr = NULL;
		}
	}
}


