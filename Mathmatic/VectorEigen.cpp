#include "stdafx.h"
#include "VectorEigen.h"


using namespace Eigen;

namespace Math
{

	VectorEigen::VectorEigen(Eigen::VectorXf vec):_vec(vec)
	{

	}

	VectorEigen::VectorEigen(const unsigned int d)
	{
		_vec = VectorXf(d);
		for (unsigned int i = 0; i < d; ++i)
		{
			_vec[i] = 0;
		}
	}

	VectorEigen::VectorEigen(std::vector<double> r)
	{
		if (r.empty()) return;

		_vec = VectorXf(r.size());
		for (unsigned int i=0;i<r.size();++i)
		{
			_vec[i] = (float)r[i];
		}
	}

	VectorEigen::VectorEigen(const int d, const double val)
	{
		_vec = VectorXf(d);
		for (int i = 0; i < d; ++i)
		{
			_vec[i] = (float)val;
		}
	}

	VectorEigen::~VectorEigen()
	{
	}

	std::vector<double> VectorEigen::GetVector() const
	{
		vector<double> res(Dimension());

		for (unsigned int i=0;i<res.size();++i)
		{
			res[i] = _vec[i];
		}

		return res;
	}

	unsigned int VectorEigen::Dimension() const
	{
		return _vec.rows();
	}

	double VectorEigen::Norm() const
	{
		return _vec.norm();
	}

	void VectorEigen::Set_ithVal(unsigned int i, double val)
	{
		_vec[i] = (float)val;
	}

	double VectorEigen::Get_ithVal(unsigned int i) const
	{
		return _vec[i];
	}

	VectorImp* VectorEigen::Normalized() const
	{
		return new VectorEigen(_vec.normalized());
	}

	void VectorEigen::Normalize()
	{
		_vec.normalize();
	}

	VectorImp* VectorEigen::Negate() const
	{
		VectorImp* res(new VectorEigen(_vec));
		for (unsigned int i=0;i<Dimension();++i)
		{
			res->Set_ithVal(i, -res->Get_ithVal(i));
		}

		return res;
	}

	float& VectorEigen::Get_ithValRef(unsigned int i)
	{
		float& res = (float&)_vec[i];
		return res;
	}

}

