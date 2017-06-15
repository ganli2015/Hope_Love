#include "stdafx.h"
#include "Common.h"

#include <windows.h>

namespace CommonTool
{

	void SleepForSeconds(const int seconds)
	{
		Sleep(seconds * 1000);
	}

	void _COMMONTOOLSINOUT TryDeletePointer(void *ptr)
	{
		if (ptr != NULL)
		{
			delete ptr;
			ptr = NULL;
		}
	}

}