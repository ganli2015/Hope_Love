#include "stdafx.h"
#include "SparseVector.h"

#include "MatrixEigen.h"
#include "VectorEigen.h"

#include "../CommonTools/CommonCompareFunction.h"

namespace Math
{
	SparseVector::SparseVector(const size_t d) :_size(d)
	{
		_vec.resize(d);
	}

	SparseVector::~SparseVector()
	{
	}

	std::vector<float> SparseVector::GetVector() const
	{
		std::vector<float> res;
		for (size_t i = 0; i < _size; ++i)
		{
			res.push_back(Get_ithVal(i));
		}

		return res;
	}

	size_t SparseVector::Dimension() const
	{
		return _size;
	}

	float SparseVector::Norm() const
	{
		return _vec.norm();
	}

	void SparseVector::Set_ithVal(size_t i, float val)
	{
		_vec.coeffRef(i) = val;
	}

	float SparseVector::Get_ithVal(size_t i) const
	{
		return _vec.coeff(i);
	}

	float& SparseVector::Get_ithValRef(size_t i)
	{
		return (float&)_vec.coeffRef(i);
	}

	VectorImp* SparseVector::Normalized() const
	{
		SparseVector *res = new SparseVector(_size);

		auto norm = this->Norm();
		for (size_t i = 0; i < _size; ++i)
		{
			res->Set_ithVal(i, this->Get_ithVal(i) / norm);;
		}

		return res;
	}

	void SparseVector::Normalize()
	{
		auto norm = this->Norm();
		for (size_t i = 0; i < _size; ++i)
		{
			Set_ithVal(i, this->Get_ithVal(i) / norm);;
		}
	}

	VectorImp* SparseVector::Negate() const
	{
		SparseVector *res = new SparseVector(_size);

		for (size_t i = 0; i < _size; ++i)
		{
			res->Set_ithVal(i, -this->Get_ithVal(i));;
		}

		return res;
	}

	VectorImp* SparseVector::Multiply(const MatrixImp* mat) const
	{
		if (Dimension() != mat->Rows())
		{
			throw logic_error("Error when a matrix multiplies a vector: dimensions are not equal");
		}

		if (CommonTool::IsType<MatrixEigen>(right))
		{
			//Cast <right>to MatrixImp, but do not Modify it!
			MatrixImp* copy = const_cast<MatrixImp*>(mat);
			MatrixEigen* matEigen = dynamic_cast<MatrixEigen*>(copy);

			Eigen::VectorXf resVec = _vec.transpose()*(matEigen->_mat);
			return new VectorEigen(resVec);
		}
		else
		{
			throw runtime_error("Not supported method.");
		}
	}

}

