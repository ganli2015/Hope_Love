#include "stdafx.h"
#include "Common.h"

#include <windows.h>

namespace CommonTool
{

	void SleepForSeconds(const int seconds)
	{
		Sleep(seconds * 1000);
	}

}