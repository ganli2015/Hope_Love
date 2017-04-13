#pragma once
#include "GrammarModel.h"

namespace DataCollection
{
	enum PartOfSpeech;
}

namespace Mind
{
	class GrammarLocal;
	class _MINDINOUT GrammarLocalModel : public GrammarModel
	{
		map<DataCollection::PartOfSpeech, shared_ptr<GrammarLocal>> _grammarLocalTable;

	public:
		GrammarLocalModel();
		~GrammarLocalModel();

		double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const;

		//////////////////////////////////////////////////////////////////////////
		//Extract local grammar distribution from sample file.("str_sample.txt")
		//////////////////////////////////////////////////////////////////////////
		void ExtractGrammarLocalDistribution();

		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes in front of.
		//////////////////////////////////////////////////////////////////////////
		double GetP_Forward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& forward) const;
		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes behind.
		//////////////////////////////////////////////////////////////////////////
		double GetP_Backward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& backward) const;

		//////////////////////////////////////////////////////////////////////////
		///Compute confidence of <pattern> considering confidence of each POS with local grammar analysis.
		//////////////////////////////////////////////////////////////////////////
		double ComputeP_GrammarLocal(const DataCollection::PartOfSpeech& curPos, const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const;

		map<double, DataCollection::PartOfSpeech> ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const;

		void ReadGrammarLocal();

	private:

	};
}


