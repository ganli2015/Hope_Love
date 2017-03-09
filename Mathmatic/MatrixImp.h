#pragma once
#include "InOut.h"

namespace Math
{
	class Vector;
	class VectorImp;

	class MatrixImp
	{
	public:
		MatrixImp();
		virtual ~MatrixImp();

		virtual unsigned int Columns()const =0;
		
		virtual unsigned int Rows() const = 0;
		
		virtual VectorImp* nthColumn(unsigned int col) const = 0;
		
		virtual VectorImp* nthRow(unsigned int row) const = 0;
		
		virtual void SetElem(int row, int col, double val) = 0;
		
		virtual double GetElem(int row, int col) const = 0;
		
		virtual void Set_jthColumn(int n, VectorImp* vec) = 0;
		
		virtual void Set_jthColumn(int n, const double val[], const int length) = 0;
		
		virtual void Set_ithRow(int n, VectorImp* vec) = 0;
		
		virtual void Set_ithRow(int n, const double val[], const int length) = 0;
		
		virtual VectorImp* Multiply(const VectorImp* right) const = 0;

		virtual MatrixImp* Multiply(const MatrixImp* right) const = 0;
		
		virtual bool Same(const MatrixImp* mat, const double tol = 1e-6) const ;
		
		virtual MatrixImp* Negate() const ;
	};
}


