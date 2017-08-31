#include "StdAfx.h"
#include "Vector.h"
#include "VectorEigen.h"
#include "SparseVector.h"
#include "iMatrix.h"

#include "../CommonTools/CommonCompareFunction.h"

namespace Math
{
	Math::Vector::Vector(const size_t d, Vector::Type type) : _type(type)
	{
		if (type==Dense)
		{
			_imp = new VectorEigen(d);
		}
		else if (type == Sparse)
		{
			_imp = new SparseVector(d);
		}
	}

	Math::Vector::Vector(std::vector<float> r) : _imp(new VectorEigen(r)), _type(Dense)
	{

	}

	Math::Vector::Vector( const int d,const double val) : _imp(new VectorEigen(d,val)), _type(Dense)
	{

	}


	Vector::Vector( const Vector& vec ) 
	{
		_type = vec._type;
		if (vec._type == Dense)
		{
			_imp = new VectorEigen(vec.Dimension());

			for (size_t i = 0; i < vec.Dimension(); ++i)
			{
				_imp->Set_ithVal(i, vec.Get_ithVal(i));
			}
		}
		else if (vec._type == Sparse)
		{
			_imp = new SparseVector(vec.Dimension());

			for (size_t i = 0; i < vec.Dimension(); ++i)
			{
				auto val = vec.Get_ithVal(i);
				if (Math::DoubleCompare(val, 0) != 0)
				{
					//Set value only when it is not zero,
					//otherwise it may break the structure of sparse vector.
					_imp->Set_ithVal(i, val);
				}
			}
		}
	}


	Vector::Vector(VectorImp* imp)
	{
		if (CommonTool::IsType<VectorEigen>(imp))
		{
			_type = Dense;
		}
		else if (CommonTool::IsType<SparseVector>(imp))
		{
			_type = Sparse;
		}
		_imp = imp;
	}

	Math::Vector::~Vector(void)
	{
		delete _imp;
	}

	std::vector<float> Math::Vector::GetVector() const
	{
		return _imp->GetVector();
	}

	size_t Math::Vector::Dimension() const
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

	void Math::Vector::Set_ithVal( size_t i,double val )
	{
		_imp->Set_ithVal(i, val);
	}

	double Math::Vector::Get_ithVal(size_t i ) const
	{
		return _imp->Get_ithVal(i);
	}

	double Math::Vector::Dot( const Vector& val ) const
	{
		return _imp->Dot(val._imp);
	}

	Vector& Math::Vector::operator=( const Vector& vec )
	{
		if (vec._type == Dense)
		{
			_imp = ConstructDenseVectorImp(vec);
		}
		else if (vec._type == Sparse)
		{
			_imp = ConstructSparseVectorImp(vec);
		}
		return *this;
	}

	float& Vector::operator[](size_t i)
	{
		return _imp->Get_ithValRef(i);
	}

	ostream& operator<<(ostream& out, const Vector& s)
	{
		out << "{";
		size_t outSize = s.Dimension() > 10 ? 10 : s.Dimension();
		for (size_t i = 0; i < outSize; ++i)
		{
			out << s.Get_ithVal(i);
			if (i != outSize - 1)
			{
				out << " ";
			}
		}

		out << "}";
		return out;
	}

	VectorImp* Vector::ConstructDenseVectorImp(Vector vec)
	{
		VectorImp* imp = new VectorEigen(vec.Dimension());

		for (size_t i = 0; i < vec.Dimension(); ++i)
		{
			imp->Set_ithVal(i, vec.Get_ithVal(i));
		}

		return imp;
	}

	VectorImp* Vector::ConstructSparseVectorImp(Vector vec)
	{
		VectorImp* imp = new SparseVector(vec.Dimension());

		for (size_t i = 0; i < vec.Dimension(); ++i)
		{
			auto val = vec.Get_ithVal(i);
			if (Math::DoubleCompare(val, 0) != 0)
			{
				//Set value only when it is not zero,
				//otherwise it may break the structure of sparse vector.
				imp->Set_ithVal(i, val);
			}
		}

		return imp;
	}

	Vector Vector::Normalized() const
	{
		VectorImp* imp= _imp->Normalized();
		Vector res(imp);
		return res;
	}

	const double Vector::operator[](size_t i) const
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

		for (size_t i=0;i<Dimension();++i)
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


	Math::Vector Vector::Multiply(const Matrix& mat) const
	{
		auto res=_imp->Multiply(mat._imp);
		return res;
	}

	Vector operator+(const Vector& left,const Vector& right) 
	{
		assert_same_int(left.Dimension(), right.Dimension());
		int n=left.Dimension();
		vector<float> newvec;
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
		vector<float> newvec;
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
		vector<float> newvec;
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
		for (size_t i=0;i<res.Dimension();++i)
		{
			res.Set_ithVal(i,res.Get_ithVal(i)/right);
		}

		return res;
	}

}

