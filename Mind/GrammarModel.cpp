#include "stdafx.h"
#include "GrammarModel.h"
#include "CommonFunction.h"


namespace Mind
{
	GrammarModel::GrammarModel()
	{
	}


	GrammarModel::~GrammarModel()
	{
	}

	void GrammarModelTrainer::OptimizeWeights()
	{
		auto samples = CommonFunction::ParseSampleSentences(_sampleFile);
		//Prepare parameters.
		//Compute possibility for each sentence for each model.
		OptWeightsParam optParam;
		for (auto sample : samples)
		{
			optParam.possi.push_back(ComputePossibility(sample));
		}

	}

	vector<double> GrammarModelTrainer::ComputePossibility(const WordList wordList) const
	{
		vector<double> res;
		for (auto model : _models)
		{
			res.push_back(model->ComputePossiblity(wordList));
		}

		return res;
	}

	double GrammarModelTrainer::OptFunc_ComputeWeights(const std::vector<double> &weights, std::vector<double> &grad, void* f_data)
	{
		OptWeightsParam *param = reinterpret_cast<OptWeightsParam*>(f_data);

		double objValue = 0.;



		return -1;
	}

}

