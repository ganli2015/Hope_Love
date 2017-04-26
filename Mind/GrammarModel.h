#pragma once
#include "InOut.h"
#include "MindObject.h"

namespace DataCollection
{
	class Word;
}

namespace Mind
{
	typedef vector<shared_ptr<DataCollection::Word>> WordList;

	//////////////////////////////////////////////////////////////////////////
	//Interface of Grammar model.
	//////////////////////////////////////////////////////////////////////////
	class GrammarModel : public MindObject
	{
	public:
		GrammarModel();
		virtual ~GrammarModel();

		//////////////////////////////////////////////////////////////////////////
		//Compute Possibility of tagging for a given sentence. 
		//////////////////////////////////////////////////////////////////////////
		virtual double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const =0;
	};

	class _MINDINOUT GrammarModelTrainer
	{
		struct OptWeightsParam
		{
			//////////////////////////////////////////////////////////////////////////
			//Each element is a set of possibility of one sample sentence.
			//The set of possibility contains different possibilities computed by different models.
			//The order is the same as <_models>.
			//////////////////////////////////////////////////////////////////////////
			vector<vector<double>> possi;
		};
		
		vector<shared_ptr<GrammarModel>> _models;
		map<shared_ptr<GrammarModel>, double> _weights;
		string _sampleFile;

	public:

		void SetSampleFilePath(const string path) { _sampleFile = path; }

		//////////////////////////////////////////////////////////////////////////
		//Add grammar model to <me>.
		//When call <OptimizeWeights> , weights will be computed for added models. 
		//////////////////////////////////////////////////////////////////////////
		void AddGrammarModel(const shared_ptr<GrammarModel> model) { _models.push_back(model); }

		void OptimizeWeights();

		//////////////////////////////////////////////////////////////////////////
		//Get weight for model.
		//Call <OptimizeWeights> first!
		//////////////////////////////////////////////////////////////////////////
		double GetModelWeight(const shared_ptr<GrammarModel> model) const { return _weights.at(model); }

	private:
		//////////////////////////////////////////////////////////////////////////
		//Compute possibility for each model.
		//////////////////////////////////////////////////////////////////////////
		vector<double> ComputePossibility(const WordList wordList) const;

		//////////////////////////////////////////////////////////////////////////
		//Objective function.
		//  (p11*w1+p12*w2+p13*w3-1)^2+(p21*w1+p22*w2+p23*w3-1)^2+...
		//////////////////////////////////////////////////////////////////////////
		static double OptFunc_ComputeWeights(const std::vector<double> &weights, std::vector<double> &grad, void* f_data);

		static double ObjFuncValue(const std::vector<double> &weights, OptWeightsParam *param);

		static double ComputeComponent(const vector<double>& weights,const vector<double>& pSeq);
		//////////////////////////////////////////////////////////////////////////
		//Compute gradients.
		//	grad1=2*p11*(p11*w1+p12*w2+p13*w3-1)+2*p21*(p21*w1+p22*w2+p23*w3-1)+...
		//////////////////////////////////////////////////////////////////////////
		static void ComputeGrad(const vector<double>& weights, const OptWeightsParam *param, vector<double>& grad);
	};
}


