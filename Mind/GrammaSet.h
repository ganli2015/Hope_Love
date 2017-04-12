#pragma once
#include "InOut.h"
#include "GrammaTree.h"
#include "GrammarPatternModel.h"

#include "../DataCollection/GrammaPattern.h"
#include "../DataCollection/Word.h"

#include "../Mathmatic/Vector.h"

#include "../MindInterface/PublicTypedef.h"

// namespace DataCollection
// {
// 	class GrammaPattern;
// }

namespace Math
{
	class MyInt;
}

namespace Mind
{
	class GrammarModel;
	class GrammarLocal;
	class GrammarFeatureModel;
	class GrammarLocalModel;
	//class GrammarPatternModel;

	struct GrammarAttribute
	{
		DataCollection::GrammarPattern pattern;
		int frequency;
	};

	class _MINDINOUT GrammarSet : public Obj<GrammarSet>
	{
	private:

		std::map<int,GrammaTree> _forwardtree;
		std::map<int,GrammaTree> _backwardtree;

		std::vector<GrammarAttribute> _patterns;

		//////////////////////////////////////////////////////////////////////////
		//Weights for different grammar models.
		//////////////////////////////////////////////////////////////////////////
		map<shared_ptr<GrammarModel>, double> _weights;

		shared_ptr<GrammarFeatureModel> _featureModel;
		shared_ptr<GrammarPatternModel> _patternModel;
		shared_ptr<GrammarLocalModel> _localModel;

		friend class GrammarPatternModel;
		friend class DataPrepareForHopeLove;
	public:
		GrammarSet(void);
		~GrammarSet(void);

		void InitializeGrammarModel();

		//////////////////////////////////////////////////////////////////////////
		///Search all grammar patterns which are sub patterns of <pattern>.
		//////////////////////////////////////////////////////////////////////////
		std::vector<DataCollection::GrammarPattern> ContainSubsequence(const DataCollection::GrammarPattern& pattern) const;
		//////////////////////////////////////////////////////////////////////////
		///Search all grammar patterns of which <pattern> is a sub grammar pattern.
		//////////////////////////////////////////////////////////////////////////
		std::vector<DataCollection::GrammarPattern> ContainedSubsequence(const DataCollection::GrammarPattern& pattern) const;

		//////////////////////////////////////////////////////////////////////////
		///Return the count of patterns in GrammarSet that is the sub pattern of <pattern>.
		//////////////////////////////////////////////////////////////////////////
		int CountOfSubsequence(const DataCollection::GrammarPattern& pattern) const;
		int GetFreqencyofPattern(const DataCollection::GrammarPattern& pattern) const;


		void AddGrammarPattern(DataCollection::GrammarPattern& pattern);

		//////////////////////////////////////////////////////////////////////////
		///Increase the frequency of <pattern> by one.
		//////////////////////////////////////////////////////////////////////////
		void IncreasePatternFreqency(const DataCollection::GrammarPattern& pattern);
		vector<DataCollection::GrammarPattern> GrammarPatternSortByFrequency() const ;

		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes in front of.
		//////////////////////////////////////////////////////////////////////////
		double GetP_Forward(const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& forward) const;
		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes behind.
		//////////////////////////////////////////////////////////////////////////
		double GetP_Backward(const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& backward) const;

		//////////////////////////////////////////////////////////////////////////
		///Compute the possibility of <pattern> in grammar sense.
		//////////////////////////////////////////////////////////////////////////
		double ComputePossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
		map<double,DataCollection::PartOfSpeech> ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const;
		double ComputeGrammarPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const;

		//////////////////////////////////////////////////////////////////////////
		///Compute confidence of <curPOS> when its previous POS is <forwardPos> and its next POS is <backwardPos>.
		///The returned value is 0 to 1.
		//////////////////////////////////////////////////////////////////////////
		double ComputeP_GrammarLocalAnalysis(const vector<shared_ptr<DataCollection::Word>>& sentence) const;

	
	private:
		void Initialize();
		std::vector<GrammarAttribute> InputGrammaPatterns(std::string filename);
		void AddPatternToTree(const DataCollection::GrammarPattern& pattern);

		//////////////////////////////////////////////////////////////////////////
		///Initialize weights for grammar models.
		//////////////////////////////////////////////////////////////////////////
		void InitializeWeights(const string filePath);

		void WriteWeights(const double wPattern, const double wLocal);
		//////////////////////////////////////////////////////////////////////////
		///Read weights from file.
		//////////////////////////////////////////////////////////////////////////
		void ReadWeights();

		int GetMaxID() const;
		int FindPatternIndex(const DataCollection::GrammarPattern& pattern);

		//////////////////////////////////////////////////////////////////////////
		///Get the sum of frequencies of all grammar patterns.
		//////////////////////////////////////////////////////////////////////////
		Math::MyInt GetTotalFrequency() const;
	};
}


