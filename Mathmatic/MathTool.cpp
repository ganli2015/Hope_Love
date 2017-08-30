#include "StdAfx.h"
#include "MathTool.h"
#include "Vector.h"
#include "iMatrix.h"
#include "Rand.h"


namespace Math
{
	int DoubleCompare( double left,double right,double tol/*=1e-6*/ )
	{
		if(left<=right+tol&&left>=right-tol)
			return 0;
		else if(left>right+tol)
			return 1;
		else 
			return -1;
	}

	int _MATHMATICINOUT FloatCompare(float left, float right, double tol /*= 1e-4*/)
	{
		if (left <= right + tol&&left >= right - tol)
			return 0;
		else if (left > right + tol)
			return 1;
		else
			return -1;
	}

	vector<int> _MATHMATICINOUT GetDicemalDigit( const double val ,const int num)
	{
		vector<int> res;
		double decimal=val-(int)val;

		for (int i=0;i<=num;++i)
		{
			decimal=decimal*10;
			res.push_back((int)decimal);
			decimal=decimal-(int)decimal;
		}

		return res;
	}

	vector<double> GenerateContinuousNumber( int from,int to )
	{
		vector<double> res;
		for (int i=from;i<=to;++i)
		{
			res.push_back(i);
		}
		return res;
	}

	double RandomDouble( int figure/*=10*/ )
	{
		double val(0);
		for (int i=1;i<=figure;++i)
		{
			int r=rand()%10001;
			srand(r);
			double ran=(rand() % (10));
			val+=ran/pow(10.0,i);
		}
		return val;
	}

	Vector ToVector( const float val[], const int n )
	{
		Vector vec(Tovector(val,n));
		return vec;
	}

	Math::Matrix CreateRandomMatrix(size_t i,size_t j )
	{	
		vector<Vector> mat;
		for (size_t n=0;n<j;++n)
		{
			Vector vec(i);
			for (size_t m=0;m<i;++m)
			{
				double val=Rand::GetRandDecimal();
				vec.Set_ithVal(m,val);
			}
			vec=vec.Normalized();
			mat.push_back(vec);
		}

		return Matrix(mat);
	}

	vector<double> _MATHMATICINOUT CreateRandomDoubleList(size_t length)
	{
		vector<double> res;
		for (size_t m = 0; m < length; ++m)
		{
			double val = Rand::GetRandDecimal();
			res.push_back(val);
		}
		return res;
	}

	double _MATHMATICINOUT ComputeDeviation(const vector<double>& vec1, const vector<double>& vec2)
	{
		Check(vec1.size()==vec2.size());

		double sqSum=0.;
		for (size_t i=0;i<vec2.size();++i)
		{
			sqSum+=pow(vec1[i]-vec2[i],2);
		}

		return sqrt(sqSum/vec1.size());
	}

	bool _MATHMATICINOUT SameVec( const vector<double>& vec1,const vector<double>& vec2 )
	{
		if(vec1.size()!=vec2.size()) return false;

		for (size_t i=0;i<vec1.size();++i)
		{
			if(DoubleCompare(vec1[i],vec2[i])!=0)
			{
				return false;
			}
		}

		return true;
	}

	bool _MATHMATICINOUT SameVec( const vector<int>& vec1,const vector<int>& vec2 )
	{
		if(vec1.size()!=vec2.size()) return false;

		for (size_t i=0;i<vec1.size();++i)
		{
			if(vec1[i]!=vec2[i])
			{
				return false;
			}
		}

		return true;
	}

	Matrix _MATHMATICINOUT ComputeMeanMatrix( const vector<Matrix>& mats )
	{
		Check(!mats.empty());

		Matrix sum(mats[0].Rows(),mats[0].Columns());
		for (size_t i=0;i<mats.size();++i)
		{
			sum+=mats[i];
		}

		return sum/mats.size();
	}

	Vector _MATHMATICINOUT ComputeMeanVector( const vector<Vector>& vecs )
	{
		Check(!vecs.empty());

		Vector sum(vecs[0].Dimension());
		for (size_t i=0;i<vecs.size();++i)
		{
			sum+=vecs[i];
		}

		return sum/vecs.size();
	}

	vector<Matrix> _MATHMATICINOUT ComputeNegateMatrix( const vector<Matrix>& mats )
	{
		vector<Matrix> res;
		res.reserve(mats.size());

		for (size_t i=0;i<mats.size();++i)
		{
			res.push_back(mats[i].Negate());
		}

		return res;
	}

	vector<Vector> _MATHMATICINOUT ComputeNegateVector( const vector<Vector>& vecs )
	{
		vector<Vector> res;
		res.reserve(vecs.size());

		for (size_t i=0;i<vecs.size();++i)
		{
			res.push_back(vecs[i].Negate());
		}

		return res;
	}

	double _MATHMATICINOUT Round( const double& val )
	{
		return (val> 0.0) ? floor(val+ 0.5) : ceil(val- 0.5);
	}

	void _MATHMATICINOUT Reverse( vector<Matrix>& mats )
	{
		vector<Matrix> copy=mats;
		mats.clear();
		mats.reserve(copy.size());

		for (vector<Matrix>::reverse_iterator it=copy.rbegin();it!=copy.rend();++it)
		{
			mats.push_back(*it);
		}
	}

	void _MATHMATICINOUT Reverse(vector<Math::Vector>& vecs)
	{
		vector<Vector> copy=vecs;
		vecs.clear();
		vecs.reserve(copy.size());

		for (vector<Vector>::reverse_iterator it=copy.rbegin();it!=copy.rend();++it)
		{
			vecs.push_back(*it);
		}
	}

}

