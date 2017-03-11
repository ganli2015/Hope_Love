#pragma once
#include "InOut.h"
#include "MatrixImp.h"

#include <Eigen/dense>

namespace Math
{
	class MatrixEigen : public MatrixImp
	{
		Eigen::MatrixXf _mat;

		friend class VectorEigen;
	public:
		MatrixEigen(const int i, const int j);
		~MatrixEigen(void);
		MatrixEigen(const std::vector<std::vector<double>> val);

		MatrixEigen(const std::vector<Vector>& val);

		virtual unsigned int Columns()const ;

		virtual unsigned int Rows() const ;

		virtual VectorImp* nthColumn(unsigned int col) const ;

		virtual VectorImp* nthRow(unsigned int row) const ;

		virtual void SetElem(int row, int col, double val) ;

		virtual double GetElem(int row, int col) const ;

		virtual void Set_jthColumn(int n, VectorImp* vec) ;

		virtual void Set_jthColumn(int n, const double val[], const int length) ;

		virtual void Set_ithRow(int n, VectorImp* vec) ;

		virtual void Set_ithRow(int n, const double val[], const int length) ;

		virtual VectorImp* Multiply(const VectorImp* right) const ;

		virtual MatrixImp* Multiply(const MatrixImp* right) const;

	private: 
		MatrixEigen(Eigen::MatrixXf mat);
	};
}

