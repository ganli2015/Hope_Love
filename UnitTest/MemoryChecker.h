#pragma once
#include <string>
#include <fstream>

#define MEMOCHECK MemoryChecker mc(__FUNCTION__)
#define RELEASE_MEMOCHECK mc.~MemoryChecker()

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

