#pragma once
#include "PublicHeader.h"

#ifndef UNIT_TEST

class TestAlgorithm :public testing::Test
{
protected:
	virtual void SetUp();
	virtual void TearDown();
};

#endif // UNIT_TEST
