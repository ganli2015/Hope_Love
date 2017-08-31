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
	public:
		enum Type
		{
			Dense,
			Sparse
		};

	private:

		VectorImp *_imp;
		Type _type;

		friend class Matrix;

	public:
		Vector(const size_t d, Vector::Type type = Vector::Dense);

		~Vector(void);

		//Always dense vector.
		Vector(std::vector<float> r);

		//Always dense vector.
		Vector(const int d, const double val);

		Vector(const Vector& vec);

		std::vector<float> GetVector() const;

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

		_MATHMATICINOUT friend ostream& operator<<(ostream& out, const Vector& s);

	private:
		Vector(VectorImp* imp);
		VectorImp* ConstructDenseVectorImp(Vector vec);
		VectorImp* ConstructSparseVectorImp(Vector vec);
	};

	_MATHMATICINOUT Vector operator+(const Vector& left,const Vector& right) ;

	_MATHMATICINOUT Vector operator-(const Vector& left,const Vector& right);

	_MATHMATICINOUT Vector operator*(const Vector& left,const double val) ;

	_MATHMATICINOUT double operator*(const Vector& left,const Vector& right) ;

	_MATHMATICINOUT Vector operator*(const double val,const Vector& vec);
}


