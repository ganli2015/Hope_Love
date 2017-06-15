#pragma once

#include <vector>
#include <map>
#include <memory>
#include <assert.h>
#include <string>
#include <math.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "../CommonTools/assertions.h"
#include "../CommonTools/MyObject.h"

using std::tr1::shared_ptr;
using namespace std;

#ifdef  MINDDATABASEAPI
#define _MINDDATABASEINOUT _declspec(dllexport)
#else
#define _MINDDATABASEINOUT _declspec(dllimport)
#endif