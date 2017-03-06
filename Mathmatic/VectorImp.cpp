#include "stdafx.h"
#include "VectorImp.h"

#include "MathTool.h"

namespace Math
{
	VectorImp::VectorImp()
	{
	}


	VectorImp::~VectorImp()
	{
	}

	double VectorImp::Angle(const VectorImp& vec)
	{
		return acos(this->Dot(vec) / this->Norm() / vec.Norm());
	}

	bool VectorImp::IsOrthogonoality(const VectorImp& vec, double tol /*= 1e-6*/)
	{
		if (DoubleCompare(this->Dot(vec), 0) == 0)
			return true;
		else return false;
	}

	double VectorImp::Dot(const VectorImp& val) const
	{
		double result(0);
		for (size_t i = 0; i < Dimension(); ++i)
		{
			result += Get_ithVal(i) * val.Get_ithVal(i);
		}
		return result;
	}

	bool VectorImp::Same(const VectorImp& v, const double tol /*= 1e-6*/) const
	{
		if (v.Dimension() !=Dimension())
		{
			return false;
		}

		for (unsigned int i = 0; i < Dimension(); ++i)
		{
			if (DoubleCompare(Get_ithVal(i), v.Get_ithVal(i), tol) != 0)
			{
				return false;
			}
		}

		return true;
	}

}

