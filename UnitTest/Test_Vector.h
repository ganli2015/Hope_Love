#pragma once
#include "PublicHeader.h"
#include "../Mathmatic/Vector.h"

struct Param_Vector
{
	shared_ptr<Math::Vector> vec;
	vector<float> fvec;

	Param_Vector(size_t size, Math::Vector::Type type) :vec(make_shared<Math::Vector>(size, type))
	{

	}
};

class Test_Vector : public::testing::TestWithParam<Param_Vector>
{
	static vector<Param_Vector> res;
public:
	static vector<Param_Vector> GenerateSamples();

private:

	static void AddSample(float array[], size_t size, Math::Vector::Type type);
};