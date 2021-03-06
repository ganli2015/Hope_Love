#pragma once
#include "InOut.h"

namespace DataCollection
{
	class Word;

	///StructuredSentence contains information of relationship of words in a sentence and how strong each pair of words is related.
	class _DATACOLLECTIONINOUT StructuredSentence : public Obj<StructuredSentence>
	{
		std::vector<shared_ptr<DataCollection::Word>> _sen;
		vector<vector<double>> _intensityMatrix; //Record the intensity of each pair of words.The matrix is symmetric.
		bool _analyzed;

	public:
		StructuredSentence(void);
		~StructuredSentence(void);
		StructuredSentence(const std::vector<shared_ptr<DataCollection::Word>>& vec);

		void SetIntensity(const size_t i,const size_t j,double intensity);
		double GetIntensity(const size_t i,const size_t j);

		///Check whether this sentence is analyzed.
		///Once <SetIntensity> is called , <Analyzed> will be true.
		bool Analyzed() const {return _analyzed;}
	private: 
		///Set all element of the matrix zero.
		void InitializeMatrix();
	};
}


