#pragma once
#include "InOut.h"

namespace Math
{
	class Vector;
	class Matrix;

	const double PI=3.1415926;
	const double e=2.718281828459;
	const int BigInt=100000000;

	//If <left> is bigger than <right>, return 1.
	//If <left> is near <right> within <tol>, return 0.
	//If <left> is smaller than <right>, return -1.
	int _MATHMATICINOUT DoubleCompare(double left,double right,double tol=1e-6);
	//If <left> is bigger than <right>, return 1.
	//If <left> is near <right> within <tol>, return 0.
	//If <left> is smaller than <right>, return -1.
	int _MATHMATICINOUT FloatCompare(float left, float right, double tol = 1e-4);

	vector<double> _MATHMATICINOUT Normalized(const vector<double> val);
	//获得val的小数部分的数字
	vector<int> _MATHMATICINOUT GetDicemalDigit(const double val,const int num);
	
	vector<double> _MATHMATICINOUT GenerateContinuousNumber(int from,int to);

	double _MATHMATICINOUT RandomDouble(int figure=10);

	Vector _MATHMATICINOUT ToVector(const float val[], const int n);

	Matrix _MATHMATICINOUT CreateRandomMatrix(size_t i,size_t j);

	//////////////////////////////////////////////////////////////////////////
	//Create a random double list of values from 0 to 1.
	//////////////////////////////////////////////////////////////////////////
	vector<double> _MATHMATICINOUT CreateRandomDoubleList(size_t length);

	double _MATHMATICINOUT ComputeDeviation(const vector<double>& vec1,const vector<double>& vec2);

	bool _MATHMATICINOUT SameVec(const vector<double>& vec1,const vector<double>& vec2);

	bool _MATHMATICINOUT SameVec(const vector<int>& vec1,const vector<int>& vec2);

	Matrix _MATHMATICINOUT ComputeMeanMatrix(const vector<Matrix>& mats);

	Vector _MATHMATICINOUT ComputeMeanVector(const vector<Vector>& vecs);

	vector<Matrix> _MATHMATICINOUT ComputeNegateMatrix(const vector<Matrix>& mats);

	vector<Vector> _MATHMATICINOUT ComputeNegateVector(const vector<Vector>& vecs);

	double _MATHMATICINOUT Round(const double& val);

	void _MATHMATICINOUT Reverse(vector<Math::Matrix>& mats);

	void _MATHMATICINOUT Reverse(vector<Math::Vector>& vecs);

	template<class T>
	double MeanValue(const vector<T>& vec)
	{
		double res=0;
		for (size_t i=0;i<vec.size();++i)
		{
			res+=vec[i];
		}

		return res/vec.size();
	}

	template<class T>
	vector<T> Tovector(const T val[], const int n)
	{
		int size=n;
		vector<T> vec;
		vec.reserve(size);
		for (int i=0;i<size;++i)
		{
			vec.push_back(val[i]);
		}
		return vec;
	}

}
