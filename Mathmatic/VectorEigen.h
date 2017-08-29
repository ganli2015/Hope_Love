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
		friend class SparseVector;
	public:
		VectorEigen(const size_t d);

		virtual ~VectorEigen(void);

		VectorEigen(std::vector<float> r);

		VectorEigen(const int d, const float val);

		virtual std::vector<float> GetVector() const ;

		virtual size_t Dimension() const ;

		virtual float Norm() const;

		virtual void Set_ithVal(size_t i, float val);
		virtual float Get_ithVal(size_t i) const ;
		virtual float& Get_ithValRef(size_t i);

		virtual VectorImp* Normalized() const;

		virtual void Normalize() ;

		virtual VectorImp* Negate() const;

		virtual VectorImp* Multiply(const MatrixImp* mat) const;

	private:
		VectorEigen(Eigen::VectorXf vec);
	};
}


