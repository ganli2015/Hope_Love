#include "stdafx.h"
#include "VectorEigen.h"


using namespace Eigen;

namespace Math
{
	VectorEigen::VectorEigen()
	{
	}


	VectorEigen::VectorEigen(Eigen::VectorXf vec):_vec(vec)
	{

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
		return _vec.cols();
	}

	double VectorEigen::Norm() const
	{
		return _vec.norm();
	}

	void VectorEigen::Set_ithVal(unsigned int i, double val)
	{
		_vec[i] = val;
	}

	double VectorEigen::Get_ithVal(unsigned int i) const
	{
		return _vec[i];
	}

	std::tr1::shared_ptr<VectorImp> VectorEigen::Normalized() const
	{
		return std::tr1::shared_ptr<VectorEigen>(new VectorEigen(_vec.normalized()));
	}

	void VectorEigen::Normalize()
	{
		_vec.normalize();
	}

	std::tr1::shared_ptr<VectorImp> VectorEigen::Negate() const
	{
		shared_ptr<VectorImp> res(new VectorEigen(_vec));
		for (unsigned int i=0;i<Dimension();++i)
		{
			res->Set_ithVal(i, -res->Get_ithVal(i));
		}

		return res;
	}

}

