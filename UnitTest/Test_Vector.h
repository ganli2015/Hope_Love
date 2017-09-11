#pragma once
#include "PublicHeader.h"
#include "../Mathmatic/Vector.h"
#include "../Mathmatic/iMatrix.h"

class TestVectorBase
{
protected:
	//Convert array to Vector.
	static shared_ptr<Math::Vector> AssignValueToVector(float array[], size_t size, Math::Vector::Type type);
};

//Test functions for single Vector.
struct Param_Vector
{
	shared_ptr<Math::Vector> vec;
	vector<float> fvec;

	friend ostream& operator<<(ostream& out, const Param_Vector& s)
	{
		out << "vec:" << *s.vec;
		return out;
	}
};

class Test_Vector : public::testing::TestWithParam<Param_Vector> , protected TestVectorBase
{
	static vector<Param_Vector> res;
public:
	static vector<Param_Vector> GenerateSamples();

protected:

	static void AddSample(float array[], size_t size, Math::Vector::Type type);
};

//Test functions for double Vectors, such as Dot and Angle.
struct Param_DoubleVector
{
	shared_ptr<Math::Vector> vec1;
	shared_ptr<Math::Vector> vec2;
	double dot;
	double angle;

	friend ostream& operator<<(ostream& out, const Param_DoubleVector& s)
	{
		out << "vec1:" << *s.vec1 << " vec2:" << *s.vec2 <<
			" dot:" << s.dot << " angle:" << s.angle;
		return out;
	}
};

class Test_DoubleVector : public::testing::TestWithParam<Param_DoubleVector>, protected TestVectorBase
{
	static vector<Param_DoubleVector> res;
public:
	static vector<Param_DoubleVector> GenerateSamples();

private:

	static void AddSample(float array1[], size_t size1, Math::Vector::Type type1,
		float array2[], size_t size2, Math::Vector::Type type2,
		double angle, double dot);
};


//TODO: Test functions for Vector and Matrix.
struct Param_VectorMatrix
{
	shared_ptr<Math::Vector> vec1;
	shared_ptr<Math::Matrix> vec2;
	double dot;
	double angle;
};

class Test_VectorMatrix : public::testing::TestWithParam<Param_VectorMatrix>, protected TestVectorBase
{
public:

private:

};