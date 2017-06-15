#include "StdAfx.h"
#include "StructuredSentence.h"
#include "Word.h"
#include "LanguageFunc.h"

namespace DataCollection
{
	StructuredSentence::StructuredSentence(void):_analyzed(false)
	{
	}


	StructuredSentence::~StructuredSentence(void)
	{
	}

	StructuredSentence::StructuredSentence( const std::vector<shared_ptr<DataCollection::Word>>& vec):_sen(vec),_analyzed(false)
	{
		InitializeMatrix();
	}

	void StructuredSentence::InitializeMatrix()
	{
		_intensityMatrix.clear();
		size_t size=_sen.size();
		for (size_t i=0;i<size;++i)
		{
			vector<double> tmpVec(size,0);
			_intensityMatrix.push_back(tmpVec);
		}
	}

	void StructuredSentence::SetIntensity( const size_t i,const size_t j,double intensity )
	{
		if(i>=_intensityMatrix.size() || j>=_intensityMatrix.size())
		{
			throw out_of_range("i or j is out of the range!!");
		}
		//I consider the intensity between words is bidirectional.
		//So the matrix is symmetric.
		_intensityMatrix[i][j]=intensity;
		_intensityMatrix[j][i]=intensity;

		_analyzed=true;
	}

	double StructuredSentence::GetIntensity( const size_t i,const size_t j )
	{
		if(i>=_intensityMatrix.size() || j>=_intensityMatrix.size())
		{
			throw out_of_range("i or j is out of the range!!");
		}

		return _intensityMatrix[i][j];
	}

}

