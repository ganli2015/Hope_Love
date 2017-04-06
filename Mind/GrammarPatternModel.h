#pragma once
#include "GrammarModel.h"
#include <unordered_map>
#include "../Mathmatic/MyInt.h"

namespace Mind
{
	class GrammarSet;
	class _MINDINOUT GrammarPatternModel : public GrammarModel
	{
		GrammarSet* _grammarSet;

		//////////////////////////////////////////////////////////////////////////
		//The total frequency of all grammar patterns.
		//////////////////////////////////////////////////////////////////////////
		Math::MyInt _totalFreq;

	public:
		GrammarPatternModel(GrammarSet* val);
		virtual ~GrammarPatternModel();

		virtual double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const ;

		//////////////////////////////////////////////////////////////////////////
		//Extract all grammar patterns from file <filename>.
		//////////////////////////////////////////////////////////////////////////
		void ExtractGrammarPatternFromInitialFile(const string filename) const;
	};

	class _MINDINOUT GrammarPatternExtractor
	{
		struct Pattern_Distribution
		{
			vector<int> pattern;
			int count;
		};

	public:
		struct Sen_Gra
		{
			//////////////////////////////////////////////////////////////////////////
			///Vector of Part of Speech.
			//////////////////////////////////////////////////////////////////////////
			vector<int> gra;
		};


	public:
		GrammarPatternExtractor();
		~GrammarPatternExtractor();

		void Run(const string file) const;

		static vector<Sen_Gra> InputGraSamples(const string file);

	private:


		void ExtractGrammarPatternFromInitialFile(const string file) const;

		unordered_map<string, int> FindAllCommonSequencesAndDistribution(const vector<Sen_Gra>& samples) const;
		vector<string> ConvertToStrings(const vector<vector<int>>& commonSeq) const;
		void OutputPatternDistribution(const unordered_map<string, int>& p_d) const;
	};
}



