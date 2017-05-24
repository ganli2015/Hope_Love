#include <vector>
#include <map>
#include <memory>
#include <assert.h>
#include <string>
#include <math.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <gtest/gtest.h>

#include "FuncForTest.h"
#include "MemoryChecker.h"
#include "../CommonTools/MyObject.h"
#include "../CommonTools/assertions.h"

using namespace std;

//Output message of expect and result when assert is failed.
#define EXPECT_RESULT(expect,result)  "Expect : " << expect<<"	"\
<< "Result type: %s." << result