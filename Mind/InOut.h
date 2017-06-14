#pragma once
#pragma warning(disable:4251)
#pragma warning(disable:4275)

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

#include "FilePath.h"

using std::tr1::shared_ptr;
using namespace std;

#ifdef  MINDAPI
#define _MINDINOUT _declspec(dllexport)
#else
#define _MINDINOUT _declspec(dllimport)
#endif

#ifdef MINDTESTEXPORT
#define _MINDTESTEXPORT _declspec(dllexport)
#else
#define _MINDTESTEXPORT _declspec(dllimport)
#endif // MINDTESTEXPORT

//#define _Extract_Initial_Grammar_Pattern
//#define _Train_Initial_React_Network
//#define _CheckInitialConceptData

namespace DataCollection
{
	class GrammarFeature;
}

namespace Mind
{
	typedef vector<shared_ptr<DataCollection::GrammarFeature>> FeatureList;
}