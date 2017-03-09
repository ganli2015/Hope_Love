#include "stdafx.h"
#include "VectorEigen.h"

#include "MatrixEigen.h"

#include "../CommonTools/CommonCompareFunction.h"
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

	VectorImp* VectorEigen::Multiply(const MatrixImp* right) const
	{
		if (Dimension() != right->Rows())
		{
			throw logic_error("Error when a matrix multiplies a vector: dimensions are not equal");
		}

		if (CommonTool::IsType<MatrixEigen>(right))
		{
			//Cast <right>to MatrixImp, but do not Modify it!
			MatrixImp* copy = const_cast<MatrixImp*>(right);
			MatrixEigen* matEigen = dynamic_cast<MatrixEigen*>(copy);

			Eigen::VectorXf resVec = _vec.transpose()*(matEigen->_mat);
			return new VectorEigen(resVec);
		}
		else
		{
			throw runtime_error("Not supported method.");
		}
	}

	float& VectorEigen::Get_ithValRef(unsigned int i)
	{
		float& res = (float&)_vec[i];
		return res;
	}

}

