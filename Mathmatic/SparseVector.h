#pragma once
#include "InOut.h"
#include "VectorImp.h"

#include <Eigen/Core>
#include <Eigen/SparseCore>


namespace Math
{
	class SparseVector : public VectorImp
	{
		Eigen::SparseVector<float> _vec;
		size_t _size;
	public:
		SparseVector(const size_t d);

		~SparseVector();

		virtual std::vector<float> GetVector() const;

		virtual size_t Dimension() const ;

		virtual float Norm() const ;

		virtual void Set_ithVal(size_t i, float val) ;

		virtual float Get_ithVal(size_t i) const ;

		virtual float& Get_ithValRef(size_t i) ;

		virtual VectorImp* Normalized() const ;

		virtual void Normalize() ;

		virtual VectorImp* Negate() const ;

		virtual VectorImp* Multiply(const MatrixImp* mat) const ;

	};
}


