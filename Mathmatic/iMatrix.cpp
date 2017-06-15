#include "StdAfx.h"
#include "iMatrix.h"

#include "MatrixEigen.h"
#include "Vector.h"

namespace Math
{
	Math::Matrix::Matrix( const int i, const int j ) :_imp(new MatrixEigen(i,j))
	{
	}

	Math::Matrix::Matrix( const std::vector<std::vector<double>> val ):_imp(new MatrixEigen(val))
	{

	}

	Math::Matrix::Matrix( const std::vector<Vector>& val ) :_imp(new MatrixEigen(val))
	{

	}

	Math::Matrix::Matrix( const Matrix& mat )
	{
		_imp = ConstructMat(mat);
	}

	Math::Matrix::~Matrix( void )
	{
		delete _imp;
	}

	Math::Vector Math::Matrix::nthColumn( size_t col ) const
	{
		return _imp->nthColumn(col);
	}

	Math::Vector Math::Matrix::nthRow( size_t row ) const
	{
		return _imp->nthRow(row);
	}

	void Math::Matrix::SetElem( int row,int col,double val )
	{
		_imp->SetElem(row, col, val);
	}

	double Math::Matrix::GetElem( int row,int col ) const
	{
		return _imp->GetElem(row, col);
	}

	void Math::Matrix::Set_jthColumn( int n, Vector vec )
	{
		_imp->Set_jthColumn(n, vec._imp);
	}

	Math::Vector Math::Matrix::Multiply( const Vector& right ) const
	{
		return _imp->Multiply(right._imp);
	}

	Matrix& Math::Matrix::operator=( const Matrix& mat )
	{
		_imp = ConstructMat(mat);
		return *this;
	}

	Vector  operator*(const Matrix& mat,const Vector& vec)
	{
		return mat.Multiply(vec);
	}

	Vector  operator*(const Vector& vec,const Matrix& mat)
	{
		assert(vec.Dimension()==mat.Rows());
		return vec.Multiply(mat);
	}

	bool Matrix::Same( const Matrix& mat,const double tol/*=1e-6*/ ) const
	{
		return _imp->Same(mat._imp, tol);
	}

	size_t Matrix::Columns() const
	{
		return _imp->Columns();
	}

	size_t Matrix::Rows() const
	{
		return _imp->Rows();
	}

	Matrix operator+( const Matrix& left,const Matrix& right )
	{
		Check(left.Rows()==right.Rows());
		Check(left.Columns()==right.Columns());

		Matrix res(left.Rows(),left.Columns());

		for (size_t i=0;i<left.Columns();++i)
		{
			Vector resColumn=left.nthColumn(i)+right.nthColumn(i);
			res.Set_jthColumn(i,resColumn);
		}

		return res;
	}

	Matrix& Matrix::operator+=( const Matrix& right )
	{
		Check(Rows()==right.Rows());
		Check(Columns()==right.Columns());

		for (size_t i=0;i<Columns();++i)
		{
			Vector resColumn=nthColumn(i)+right.nthColumn(i);
			Set_jthColumn(i,resColumn);
		}

		return *this;
	}

	Matrix::Matrix(MatrixImp* imp):_imp(imp)
	{

	}

	MatrixImp* Matrix::ConstructMat(const Matrix& mat) const
	{
		MatrixEigen* res = new MatrixEigen(mat.Rows(), mat.Columns());
		for (size_t i = 0; i < mat.Rows(); ++i)
		{
			for (size_t j = 0; j < mat.Columns(); ++j)
			{
				res->SetElem(i, j, mat.GetElem(i, j));
			}
		}

		return res;
	}

	Matrix Matrix::Negate() const
	{
		return _imp->Negate();
	}

	void Matrix::Set_jthColumn( int n, const double val[],const int length )
	{
		_imp->Set_jthColumn(n, val, length);
	}

	void Matrix::Set_ithRow( int n, Vector vec )
	{
		_imp->Set_ithRow(n, vec._imp);
	}

	void Matrix::Set_ithRow( int n, const double val[] ,const int length)
	{
		_imp->Set_ithRow(n, val, length);
	}

	Matrix operator*( const Matrix& left,const Matrix& right )
	{
		return left._imp->Multiply(right._imp);
	}

	Matrix operator*( const Matrix& left,const double& right )
	{
		Matrix res(left);
		for (size_t i=0;i<res.Columns();++i)
		{
			Vector newCol=res.nthColumn(i)*right;
			res.Set_jthColumn(i,newCol);
		}

		return res;
	}


	Matrix operator/( const Matrix& left,const double& right )
	{
		Check(DoubleCompare(right,0)!=0);

		Matrix res(left);
		for (size_t i=0;i<res.Columns();++i)
		{
			Vector newCol=res.nthColumn(i)/right;
			res.Set_jthColumn(i,newCol);
		}

		return res;
	}

}




