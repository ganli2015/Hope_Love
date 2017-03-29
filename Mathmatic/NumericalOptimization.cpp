#include "stdafx.h"
#include "NumericalOptimization.h"

#include <nlopt.hpp>

namespace Math
{
	NumericalOptimization::NumericalOptimization(const algorithm alg, const unsigned n):_opt(new nlopt::opt((nlopt::algorithm)alg,n))
	{
		
	}


	NumericalOptimization::~NumericalOptimization()
	{
		delete _opt;
	}

	void NumericalOptimization::SetObjectiveFunction(ObjectFunction objFunc, void* otherParam)
	{
		_opt->set_min_objective(objFunc, otherParam);
	}

	void NumericalOptimization::SetInequalityConstraint(ConstraintFunc func, void* otherParam /*= NULL*/, double tol /*= 0*/)
	{
		_opt->add_inequality_constraint(func, otherParam, tol);
	}

	void NumericalOptimization::SetEqualityConstraint(ConstraintFunc func, void* otherParam /*= NULL*/, double tol /*= 0*/)
	{
		_opt->add_equality_constraint(func, otherParam, tol);
	}

	void NumericalOptimization::SetXTol(const double xtol)
	{
		_opt->set_xtol_abs(xtol);
	}

	void NumericalOptimization::SetFTol(const double ftol)
	{
		_opt->set_ftol_abs(ftol);
	}

	void NumericalOptimization::SetMaxIteration(const unsigned maxIter)
	{
		_opt->set_maxeval(maxIter);
	}

	Math::OptResult NumericalOptimization::Optimize(std::vector<double> &x, double &opt_f)
	{
		return (OptResult)_opt->optimize(x, opt_f);
	}

}

