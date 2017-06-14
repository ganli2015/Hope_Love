#pragma once
#include "StdAfx.h"
#include "InOut.h"
#include "Vector.h"

namespace Math
{
	class get_nthElemOfVector
	{
		int _n;
		std::vector<double> _vec;
	public:
		get_nthElemOfVector(int n):_n(n){}
		double operator()(Vector vector) const 
		{
			return vector[_n];
		}
	};

	class MatrixImp;
 
	class _MATHMATICINOUT Matrix : public Obj<Matrix>
	{
		MatrixImp* _imp;

		friend class Vector;
	public:
		//////////////////////////////////////////////////////////////////////////
		///Create a matrix with i rows and j columns.
		//////////////////////////////////////////////////////////////////////////
		Matrix(const int i, const int j);
		~Matrix(void);
		Matrix(const std::vector<std::vector<double>> val);

		Matrix(const std::vector<Vector>& val);

		Matrix(const Matrix& mat);


		size_t Columns()const;

		size_t Rows() const;

		Vector nthColumn(size_t col) const;

		Vector nthRow(size_t row) const;

		void SetElem(int row,int col,double val);

		double GetElem(int row,int col) const;

		void Set_jthColumn(int n, Vector vec);

		void Set_jthColumn(int n, const double val[],const int length);

		void Set_ithRow(int n, Vector vec);

		void Set_ithRow(int n, const double val[],const int length);

		Vector Multiply(const Vector& right) const;

		bool Same(const Matrix& mat,const double tol=1e-6) const ;

		Matrix& operator=(const Matrix& mat);
		
		Matrix& operator+=(const Matrix& right);

		Matrix Negate() const ;

		_MATHMATICINOUT friend Matrix operator+(const Matrix& left,const Matrix& right);

		_MATHMATICINOUT friend Matrix operator/(const Matrix& left,const double& right);

		_MATHMATICINOUT friend Matrix operator*(const Matrix& left,const double& right);

		_MATHMATICINOUT friend Matrix operator*(const Matrix& left,const Matrix& right);

	private:

		Matrix(MatrixImp* imp);
		MatrixImp* ConstructMat(const Matrix& mat) const;
	};

	_MATHMATICINOUT Vector  operator*(const Matrix& mat,const Vector& vec);

	_MATHMATICINOUT Vector  operator*(const Vector& vec,const Matrix& mat);

}


