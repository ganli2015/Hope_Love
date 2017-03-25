#include "stdafx.h"
#include "Test_GrammarFeatureDatabase.h"

#include "../Mind/GrammarFeatureDatabase.h"

#include <sqlite3pp/sqlite3pp.h>

typedef Mind::GrammarFeatureDatabase<sqlite3pp::database> GrammarFeatureDatabase;

TEST(Test_GrammarFeatureDatabase, tt)
{
	GrammarFeatureDatabase db;
}