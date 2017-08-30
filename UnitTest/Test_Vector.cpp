#include "stdafx.h"
#include "Test_Vector.h"
#include "FuncForTest.h"
#include "../Mathmatic/MathTool.h"

using namespace Math;

INSTANTIATE_TEST_CASE_P(Test_Math, Test_Vector, testing::ValuesIn(Test_Vector::GenerateSamples()));

TEST_P(Test_Vector, GetVector)
{
	auto param = GetParam();

	auto getVector = param.vec->GetVector();

	ASSERT_TRUE(FuncForTest::ContainSameElements<float>(param.fvec, getVector));
}

TEST_P(Test_Vector, Dimension)
{
	auto param = GetParam();

	ASSERT_EQ(param.fvec.size(), param.vec->Dimension());
}

vector<Param_Vector> Test_Vector::GenerateSamples()
{
	{
		float fArray[5] = { 1,0,3,0,5 };
		AddSample(fArray, 5, Vector::Dense);
		AddSample(fArray, 5, Vector::Sparse);
	}
	{
		float fArray[10] = { 1,0,3,0,5,0,0,30,0,-40 };
		AddSample(fArray, 10, Vector::Dense);
		AddSample(fArray, 10, Vector::Sparse);
	}

	return res;
}

void Test_Vector::AddSample(float array[], size_t size, Vector::Type type)
{
	Param_Vector param(size, type);

	auto fvec = Tovector(array, size);
	param.fvec = fvec;
	Math::Vector vec(size, type);
	for (size_t i = 0; i < fvec.size(); ++i)
	{
		if (FloatCompare(fvec[i], 0) != 0)
		{
			param.vec->Set_ithVal(i, fvec[i]);
		}
	}

	res.push_back(param);
}

vector<Param_Vector> Test_Vector::res;
