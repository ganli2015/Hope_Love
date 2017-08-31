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

TEST_P(Test_Vector, Norm)
{
	auto param = GetParam();

	double expectNorm = 0;
	for (auto f : param.fvec)
	{
		expectNorm += f*f;
	}
	expectNorm = sqrt(expectNorm);

	ASSERT_FLOAT_EQ(expectNorm, param.vec->Norm());
}

TEST_P(Test_Vector, Normalized)
{
	auto param = GetParam();

	vector<float> expect = Math::SquareNormalized(param.fvec);

	ASSERT_TRUE(FuncForTest::ContainSameElements<float>(param.vec->Normalized().GetVector(), expect));
}

TEST_P(Test_Vector, Normalize)
{
	auto param = GetParam();

	Vector copyVec(*param.vec);
	copyVec.Normalize();
	vector<float> expect = Math::SquareNormalized(param.fvec);

	ASSERT_TRUE(FuncForTest::ContainSameElements<float>(copyVec.GetVector(), expect));
}

TEST_P(Test_Vector, Same)
{
	auto param = GetParam();

	Vector copyVec(*param.vec);

	ASSERT_TRUE(param.vec->Same(copyVec));
}

TEST_P(Test_Vector, NotSame)
{
	auto param = GetParam();

	Vector copyVec(*param.vec);
	copyVec.Set_ithVal(1, 0.7);

	ASSERT_TRUE(!param.vec->Same(copyVec));
}

TEST_P(Test_Vector, Negate)
{
	auto param = GetParam();

	Vector negated = param.vec->Negate();
	vector<float> expect;
	for (auto f : param.fvec)
	{
		expect.push_back(-f);
	}

	ASSERT_TRUE(FuncForTest::ContainSameElements<float>(negated.GetVector(), expect));
}

INSTANTIATE_TEST_CASE_P(Test_Math, Test_DoubleVector, testing::ValuesIn(Test_DoubleVector::GenerateSamples()));

TEST_P(Test_DoubleVector, Dot)
{
	auto param = GetParam();

	double result = param.vec1->Dot(*param.vec2);

	ASSERT_FLOAT_EQ(param.dot, result);
}

TEST_P(Test_DoubleVector, Angle)
{
	auto param = GetParam();

	double result = param.vec1->Angle(*param.vec2);

	ASSERT_FLOAT_EQ(param.angle, result);
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
	Param_Vector param;

	auto fvec = Tovector(array, size);
	param.fvec = fvec;
	param.vec = AssignValueToVector(array, size, type);

	res.push_back(param);
}

vector<Param_Vector> Test_Vector::res;

vector<Param_DoubleVector> Test_DoubleVector::GenerateSamples()
{
	{
		float fArray1[5] = { 1,0,3,0,5 };
		float fArray2[5] = { 0,0.5,5,0,2 };
		float dot = 25.0f;
		float angle = 0.67397768508769545586695527316f;
		AddSample(fArray1, 5, Vector::Dense, fArray2, 5, Vector::Dense, angle, dot);
		AddSample(fArray1, 5, Vector::Sparse, fArray2, 5, Vector::Dense, angle, dot);
		AddSample(fArray1, 5, Vector::Sparse, fArray2, 5, Vector::Sparse, angle, dot);
	}

	return res;
}

void Test_DoubleVector::AddSample(float array1[], size_t size1, Math::Vector::Type type1, 
	float array2[], size_t size2, Math::Vector::Type type2,
	double angle,double dot)
{
	Param_DoubleVector param;
	param.vec1 = AssignValueToVector(array1, size1, type1);
	param.vec2 = AssignValueToVector(array2, size2, type2);
	param.angle = angle;
	param.dot = dot;

	res.push_back(param);
}

vector<Param_DoubleVector> Test_DoubleVector::res;

shared_ptr<Math::Vector> TestVectorBase::AssignValueToVector(float array[], size_t size, Vector::Type type)
{
	shared_ptr<Math::Vector> vec = make_shared<Vector>(size);
	auto fvec = Tovector(array, size);
	for (size_t i = 0; i < fvec.size(); ++i)
	{
		if (FloatCompare(fvec[i], 0) != 0)
		{
			vec->Set_ithVal(i, fvec[i]);
		}
	}

	return vec;
}
