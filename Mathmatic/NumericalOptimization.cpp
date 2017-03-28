#include "stdafx.h"
#include "NumericalOptimization.h"

#include <nlopt/nlopt.h>

namespace Mind
{
	NumericalOptimization::NumericalOptimization():_opt(new nlopt_opt())
	{

	}


	NumericalOptimization::~NumericalOptimization()
	{
	}

	void NumericalOptimization::SetObjectiveFunction(function<double(unsigned, const double*, double, void*)> objFunc)
	{
		//nlopt_set_min_objective(_opt, objFunc, NULL);
	}

}

