#include "StdAfx.h"
#include "Vector.h"
#include "VectorEigen.h"

namespace Math
{
	Math::Vector::Vector( const unsigned int d ) :_imp(new VectorEigen(d))
	{

	}

	Math::Vector::Vector( std::vector<double> r ) : _imp(new VectorEigen(r))
	{

	}

	Math::Vector::Vector( const int d,const double val ) : _imp(new VectorEigen(d,val))
	{

	}


	Vector::Vector( const Vector& vec ) 
	{
		_imp = new VectorEigen(vec.Dimension());

		for (unsigned int i = 0; i < vec.Dimension(); ++i)
		{
			_imp->Set_ithVal(i, vec.Get_ithVal(i));
		}
	}


	Vector::Vector(VectorImp* imp)
	{
		_imp = imp;
	}

	Math::Vector::~Vector(void)
	{
		delete _imp;
	}

	std::vector<double> Math::Vector::GetVector() const
	{
		return _imp->GetVector();
	}

	unsigned int Math::Vector::Dimension() const
	{
		return _imp->Dimension();
	}

	double Math::Vector::Norm() const
	{
		return _imp->Norm();
	}

	double Math::Vector::Angle( const Vector& vec )
	{
		return _imp->Angle(vec._imp);
	}

	bool Math::Vector::IsOrthogonoality( const Vector& vec,double tol/*=1e-6*/ )
	{
		return _imp->IsOrthogonoality(vec._imp, tol);
	}

	void Math::Vector::Set_ithVal( unsigned int i,double val )
	{
		_imp->Set_ithVal(i, val);
	}

	double Math::Vector::Get_ithVal(unsigned int i ) const
	{
		return _imp->Get_ithVal(i);
	}

	double Math::Vector::Dot( const Vector& val ) const
	{
		return _imp->Dot(val._imp);
	}

	Vector& Math::Vector::operator=( const Vector& vec )
	{
		_imp = ConstructVectorImp(vec);
		return *this;
	}

	float& Vector::operator[](unsigned int i)
	{
		return _imp->Get_ithValRef(i);
	}

	VectorImp* Vector::ConstructVectorImp(Vector vec)
	{
		VectorImp* imp = new VectorEigen(vec.Dimension());

		for (unsigned int i = 0; i < vec.Dimension(); ++i)
		{
			imp->Set_ithVal(i, vec.Get_ithVal(i));
		}

		return imp;
	}

	Vector Vector::Normalized() const
	{
		VectorImp* imp= _imp->Normalized();
		Vector res(imp);
		return res;
	}

	const double Vector::operator[](unsigned int i) const
	{
		return _imp->Get_ithVal(i);
	}

	void Vector::Normalize()
	{
		_imp->Normalize();
	}

	bool Vector::Same( const Vector& v ,const double tol) const
	{
		return _imp->Same(v._imp, tol);
	}

	Vector& Vector::operator+=( const Vector& right )
	{
		Check(Dimension()==right.Dimension());

		for (unsigned int i=0;i<Dimension();++i)
		{
			double old = _imp->Get_ithVal(i);
			_imp->Set_ithVal(i, old + right.Get_ithVal(i));
		}

		return *this;
	}

	Math::Vector Vector::Negate() const
	{
		return _imp->Negate();
	}

	Vector operator+(const Vector& left,const Vector& right) 
	{
		assert_same_int(left.Dimension(),right.Dimension());
		int n=left.Dimension();
		vector<double> newvec;
		newvec.reserve(n);
		for (int i=0;i<n;++i)
		{
			newvec.push_back(left[i]+right[i]);
		}
		return Vector(newvec);
	}

	Vector operator-(const Vector& left,const Vector& right) 
	{
		assert_same_int(left.Dimension(),right.Dimension());
		int n=left.Dimension();
		vector<double> newvec;
		newvec.reserve(n);
		for (int i=0;i<n;++i)
		{
			newvec.push_back(left[i]-right[i]);
		}
		return Vector(newvec);
	}

	Vector operator*(const Vector& left,const double val) 
	{
		int n=left.Dimension();
		vector<double> newvec;
		newvec.reserve(n);
		for (int i=0;i<n;++i)
		{
			newvec.push_back(left[i]*val);
		}
		return Vector(newvec);
	}

	double operator*(const Vector& left,const Vector& right) 
	{
		assert_same_int(left.Dimension(),right.Dimension());
		int n=left.Dimension();
		return left.Dot(right);
	}

	Vector operator*(const double val,const Vector& vec)
	{
		return vec*val;
	}


	Vector operator/( const Vector& left,const double& right )
	{
		Check(DoubleCompare(right,0)!=0);

		Vector res(left);
		for (unsigned int i=0;i<res.Dimension();++i)
		{
			res.Set_ithVal(i,res.Get_ithVal(i)/right);
		}

		return res;
	}

}

