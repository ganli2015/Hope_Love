#include "StdAfx.h"
#include "MemoryChecker.h"
#include "../CommonTools/MyObject.h"
#include "../CommonTools/assertions.h"
#include "../CommonTools/MemoryDetector.h"

#include <gTest/gtest.h>


MemoryChecker::MemoryChecker(void)
{
	_detector = new CommonTool::MemoryDetector();
	_detector->Snapshot();
}

MemoryChecker::MemoryChecker( const std::string function )
{
	_detector = new CommonTool::MemoryDetector();
	_detector->Snapshot();
}


MemoryChecker::~MemoryChecker(void)
{
	_detector->Snapshot();
	if (!_detector->UnchangedSnapshot())
	{
		//throw runtime_error("Memory leak!!");
		cout << "Memory leak!!" << endl;
	}
	delete _detector;
}
