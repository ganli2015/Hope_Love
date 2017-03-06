#pragma once
#include "InOut.h"

namespace Math
{
	class Vector;

	class VectorImp
	{
	public:
		VectorImp();
		~VectorImp();

	public:
		virtual std::vector<double> GetVector() const =0;

		virtual unsigned int Dimension() const = 0;

		virtual double Norm() const = 0;

		virtual double Angle(const VectorImp& vec);

		virtual bool IsOrthogonoality(const VectorImp& vec, double tol = 1e-6) ;

		virtual void Set_ithVal(unsigned int i, double val) = 0;
		virtual double Get_ithVal(unsigned int i) const = 0;

		virtual double Dot(const VectorImp& val) const;

		virtual std::tr1::shared_ptr<VectorImp> Normalized() const = 0;

		virtual void Normalize() = 0;

		virtual bool Same(const VectorImp& v, const double tol = 1e-6) const ;

		virtual std::tr1::shared_ptr<VectorImp> Negate() const=0 ;
	};
}


