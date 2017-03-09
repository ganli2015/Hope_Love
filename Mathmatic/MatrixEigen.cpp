#include "stdafx.h"
#include "MatrixEigen.h"
#include "Vector.h"
#include "VectorEigen.h"

#include "../CommonTools/CommonCompareFunction.h"

using namespace Eigen;
namespace Math
{
	MatrixEigen::MatrixEigen(const int i, const int j)
	{
		_mat = MatrixXf(i, j);
		_mat.setZero();
	}

	MatrixEigen::MatrixEigen(const std::vector<std::vector<double>> val)
	{
		unsigned int i = val[0].size();
		unsigned int j = val.size();
		_mat = MatrixXf(i, j);
		for (unsigned int m=0;m<j;++m)
		{
			for (unsigned int n=0;n<i;++i)
			{
				_mat(n, m) = (float)val[m][n];
			}
		}
	}

	MatrixEigen::MatrixEigen(const std::vector<Vector>& val)
	{
		unsigned int i = val[0].Dimension();
		unsigned int j = val.size();
		_mat = MatrixXf(i, j);

		for (unsigned int m = 0; m < j; ++m)
		{
			for (unsigned int n = 0; n < i; ++n)
			{
				_mat(n, m) = (float)val[m][n];
			}
		}
	}

	MatrixEigen::MatrixEigen(Eigen::MatrixXf mat):_mat(mat)
	{

	}

	MatrixEigen::~MatrixEigen()
	{
	}

	unsigned int MatrixEigen::Columns() const
	{
		return _mat.cols();
	}

	unsigned int MatrixEigen::Rows() const
	{
		return _mat.rows();
	}

	Math::VectorImp* MatrixEigen::nthColumn(unsigned int col) const
	{
		auto column = _mat.col(col);
		VectorEigen* vecEigen = new VectorEigen(column);
		return vecEigen;
	}

	VectorImp* MatrixEigen::nthRow(unsigned int row) const
	{
		auto rowVec = _mat.row(row);
		return new VectorEigen(rowVec);
	}

	void MatrixEigen::SetElem(int row, int col, double val)
	{
		_mat(row, col) = (float)val;
	}

	double MatrixEigen::GetElem(int row, int col) const
	{
		return _mat(row, col);
	}

	void MatrixEigen::Set_jthColumn(int n, VectorImp* vec)
	{
		for (unsigned i=0;i<Rows();++i)
		{
			_mat(i, n) = (float)vec->Get_ithVal(i);
		}
	}

	void MatrixEigen::Set_jthColumn(int n, const double val[], const int length)
	{
		for (int i = 0; i <length; ++i)
		{
			_mat(i, n) = (float)val[i];
		}
	}

	void MatrixEigen::Set_ithRow(int n, VectorImp* vec)
	{
		for (unsigned i = 0; i < Columns(); ++i)
		{
			_mat(n, i) = (float)vec->Get_ithVal(i);
		}
	}

	void MatrixEigen::Set_ithRow(int n, const double val[], const int length)
	{
		for (int i = 0; i < length; ++i)
		{
			_mat(n, i) = (float)val[i];
		}
	}

	VectorImp* MatrixEigen::Multiply(const VectorImp* right) const
	{
		if (Columns() != right->Dimension())
		{
			throw logic_error("Error when a matrix multiplies a vector: dimensions are not equal");
		}

		if (CommonTool::IsType<VectorEigen>(right))
		{
			//Cast <right>to VectorEigen, but do not Modify it!
			VectorImp* copy = const_cast<VectorImp*>(right);
			VectorEigen* vecEigen = dynamic_cast<VectorEigen*>(copy);

			Eigen::VectorXf resVec= _mat*(vecEigen->_vec);
			return new VectorEigen(resVec);
		}
		else
		{
			throw runtime_error("Not supported method.");
		}
	}

	MatrixImp* MatrixEigen::Multiply(const MatrixImp* right) const
	{
		if (Columns() != right->Rows() )
		{
			throw logic_error("Error when a matrix multiplies a matrix: dimensions are not equal");
		}

		if (CommonTool::IsType<MatrixEigen>(right))
		{
			//Cast <right>to MatrixEigen, but do not Modify it!
			MatrixImp* copy = const_cast<MatrixImp*>(right);
			MatrixEigen* matEigen = dynamic_cast<MatrixEigen*>(copy);

			Eigen::MatrixXf resMat = _mat*(matEigen->_mat);
			return new MatrixEigen(resMat);
		}
		else
		{
			throw runtime_error("Not supported method.");
		}
	}

}

