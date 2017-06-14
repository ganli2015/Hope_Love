#pragma once
#include "InOut.h"
#include "VectorImp.h"

#include <Eigen/dense>

using namespace std::tr1;

namespace Math
{
	class MatrixImp;

	class VectorEigen : public VectorImp
	{
		Eigen::VectorXf _vec;

		friend class MatrixEigen;
	public:
		VectorEigen(const size_t d);

		virtual ~VectorEigen(void);

		VectorEigen(std::vector<double> r);

		VectorEigen(const int d, const double val);

		virtual std::vector<double> GetVector() const ;

		virtual size_t Dimension() const ;

		virtual double Norm() const;

		virtual void Set_ithVal(size_t i, double val);
		virtual double Get_ithVal(size_t i) const ;
		virtual float& Get_ithValRef(size_t i);

		virtual VectorImp* Normalized() const;

		virtual void Normalize() ;

		virtual VectorImp* Negate() const;

		virtual VectorImp* Multiply(const MatrixImp* mat) const;

	private:
		VectorEigen(Eigen::VectorXf vec);
	};
}


