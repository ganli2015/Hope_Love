#pragma once
#pragma warning(disable:4251)

#include <memory>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "../CommonTools/assertions.h"
#include "../CommonTools/MyObject.h"

using namespace std;
using std::tr1::shared_ptr;

#ifdef MATHMATICAPI
#define _MATHMATICINOUT _declspec(dllexport)
#else
#define _MATHMATICINOUT _declspec(dllimport)
#endif

