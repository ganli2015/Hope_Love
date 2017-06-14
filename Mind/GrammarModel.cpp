#include "stdafx.h"
#include "GrammarModel.h"
#include "CommonFunction.h"

#include "../Mathmatic/MathTool.h"
#include "../Mathmatic/NumericalOptimization.h"

#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"

using namespace CommonTool;

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
			auto curPossi = ComputePossibility(sample);
			optParam.possi.push_back(curPossi);
			for (auto p : curPossi)
			{
				if (_isnan(p))
				{
					ERRORLOG("NAN!!!!");
					ERRORLOG(sample);
				}
			}
		}

		//Compute random initial weights.
		vector<double> weights = Math::CreateRandomDoubleList(_models.size());
		Math::NumericalOptimization opt(weights.size());
		opt.SetObjectiveFunction(OptFunc_ComputeWeights, &optParam);
		opt.SetLowerBound(0);
		opt.SetXTol(1e-10);
		opt.SetFTol(1e-10);
		opt.SetMaxIteration(1000);
		double objFunValue = 0;
		try
		{
			LOG("Start optimization.");
			LOG_FORMAT("Initial objective function value is %lf.", ObjFuncValue(weights, &optParam));
			auto result = opt.Optimize(weights, objFunValue);
			LOG_FORMAT("Optimization result: %s.", CommonTool::ToString(result).c_str());
		}
		catch (const std::exception& ex)
		{
			LOG_EXCEPTION(ex);
		}
		LOG_FORMAT("Final objective function value is %lf.", objFunValue);
		LOG("Finish optimization.");
		//The order of weights is determined by order of <_models>
		for (unsigned i = 0; i < _models.size(); ++i)
		{
			_weights[_models[i]] = weights[i];
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
		//Compute objective function.
		double objValue = ObjFuncValue(weights,param);

		if (!grad.empty())
		{
			ComputeGrad(weights, param, grad);
		}

		return objValue;
	}

	double GrammarModelTrainer::ObjFuncValue(const std::vector<double> &weights, OptWeightsParam *param)
	{
		double objValue = 0;
		for (auto pSeq : param->possi)
		{
			double deviation = ComputeComponent(weights, pSeq);
			objValue += deviation*deviation;
		}
		return objValue;
	}

	double GrammarModelTrainer::ComputeComponent(const vector<double>& weights, const vector<double>& pSeq)
	{
		double deviation = -1;
		for (unsigned i = 0; i < pSeq.size(); ++i)
		{
			deviation += pSeq[i] * weights[i];
		}

		return deviation;
	}

	void GrammarModelTrainer::ComputeGrad(const vector<double>& weights, const OptWeightsParam *param,vector<double>& grad)
	{
		for (unsigned i = 0; i < grad.size(); ++i)
		{
			double gradVal = 0;
			for (auto pSeq : param->possi)
			{
				double comp = ComputeComponent(weights, pSeq);
				gradVal += 2 * pSeq[i] * comp;
			}
			grad[i] = gradVal;
		}
	}

}

