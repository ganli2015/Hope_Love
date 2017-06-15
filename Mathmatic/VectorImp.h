#pragma once
#include "InOut.h"

namespace Math
{
	class Vector;
	class MatrixImp;

	class VectorImp
	{
	public:
		VectorImp();
		virtual ~VectorImp();

	public:
		virtual std::vector<double> GetVector() const =0;

		virtual size_t Dimension() const = 0;

		virtual double Norm() const = 0;

		virtual void Set_ithVal(size_t i, double val) = 0;
		virtual double Get_ithVal(size_t i) const = 0;
		//////////////////////////////////////////////////////////////////////////
		//Get the reference of an element.
		//////////////////////////////////////////////////////////////////////////
		virtual float& Get_ithValRef(size_t i) = 0;

		virtual VectorImp* Normalized() const = 0;

		virtual void Normalize() = 0;

		virtual VectorImp* Negate() const=0 ;

		virtual bool Same(const VectorImp* v, const double tol = 1e-6) const;

		virtual double Dot(const VectorImp* val) const;

		virtual double Angle(const VectorImp* vec);

		virtual bool IsOrthogonoality(const VectorImp* vec, double tol = 1e-6);

		virtual VectorImp* Multiply(const MatrixImp* mat) const =0;

	};
}


