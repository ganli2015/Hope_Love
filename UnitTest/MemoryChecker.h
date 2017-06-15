#pragma once
#include <string>
#include <fstream>

#include "PublicHeader.h"

#ifdef _DEBUG
#define MEMOCHECK
#else
#define MEMOCHECK MemoryChecker mc(__FUNCTION__)
#endif // _DEBUG


namespace CommonTool
{
	class MemoryDetector;
}

class MemoryChecker
{
	CommonTool::MemoryDetector* _detector;
public:
	MemoryChecker(void);
	MemoryChecker(const std::string function);
	~MemoryChecker(void);
};

