#include "stdafx.h"
#include "MatrixImp.h"

#include "MathTool.h"
#include "MatrixEigen.h"

namespace Math
{
	MatrixImp::MatrixImp()
	{
	}


	MatrixImp::~MatrixImp()
	{
	}

	bool MatrixImp::Same(const MatrixImp* mat, const double tol /*= 1e-6*/) const
	{
		if (Rows() != mat->Rows() || Columns() != mat->Columns())
		{
			return false;
		}


		for (size_t i=0;i<mat->Rows();++i)
		{
			for (size_t j=0;j<mat->Columns();++j)
			{
				if (DoubleCompare(mat->GetElem(i, j), GetElem(i, j), tol) != 0)
				{
					return false;
				}
			}
		}
		return true;
	}

	MatrixImp* MatrixImp::Negate() const
	{
		MatrixEigen* res = new MatrixEigen(Rows(), Columns());
		for (size_t i = 0; i < Rows(); ++i)
		{
			for (size_t j = 0; j < Columns(); ++j)
			{
				res->SetElem(i, j, -GetElem(i, j));
			}
		}

		return res;
	}

}

