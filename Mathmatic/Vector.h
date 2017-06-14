#pragma once
#include "StdAfx.h"
#include "InOut.h"
#include "MathTool.h"

namespace Math
{
	using namespace std;

	class VectorImp;

	class _MATHMATICINOUT Vector : public Obj<Vector>
	{
		VectorImp *_imp;

		friend class Matrix;
	public:
		Vector(const size_t d);

		~Vector(void);

		Vector(std::vector<double> r);

		Vector(const Vector& vec);

		Vector(const int d,const double val);

		std::vector<double> GetVector() const;

		size_t Dimension() const;

		double Norm() const;

		double Angle(const Vector& vec);

		bool IsOrthogonoality(const Vector& vec,double tol=1e-6);

		void Set_ithVal(size_t i,double val);
		double Get_ithVal(size_t i) const;

		double Dot(const Vector& val) const;

		Vector Normalized() const;

		void Normalize();

		bool Same(const Vector& v ,const double tol=1e-6) const ;

		Vector Negate() const ;

		Vector Multiply(const Matrix& mat) const;

		float& operator[]( size_t i);

		const double operator[]( size_t i) const;

		Vector& operator=(const Vector& vec);

		Vector& operator+=(const Vector& right);

		_MATHMATICINOUT friend Vector operator/(const Vector& left,const double& right);

	private:
		Vector(VectorImp* imp);
		VectorImp* ConstructVectorImp(Vector vec);
	};

	_MATHMATICINOUT Vector operator+(const Vector& left,const Vector& right) ;

	_MATHMATICINOUT Vector operator-(const Vector& left,const Vector& right);

	_MATHMATICINOUT Vector operator*(const Vector& left,const double val) ;

	_MATHMATICINOUT double operator*(const Vector& left,const Vector& right) ;

	_MATHMATICINOUT Vector operator*(const double val,const Vector& vec);
}


