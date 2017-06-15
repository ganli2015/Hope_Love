#include "stdafx.h"
#include "NumericalOptimization.h"

#include <nlopt.hpp>

namespace Math
{
	NumericalOptimization::NumericalOptimization(const unsigned n, const algorithm alg):_opt(new nlopt::opt((nlopt::algorithm)alg,n))
	{
		_opt->set_maxeval(200);
		_opt->set_ftol_abs(1e-4);
		_opt->set_xtol_abs(1e-4);
	}


	NumericalOptimization::~NumericalOptimization()
	{
		delete _opt;
	}

	void NumericalOptimization::SetObjectiveFunction(ObjectFunction objFunc, void* otherParam)
	{
		_opt->set_min_objective(objFunc, otherParam);
	}

	void NumericalOptimization::SetMaximizeObjectiveFunction(ObjectFunction objFunc, void* otherParam /*= NULL*/)
	{
		_opt->set_max_objective(objFunc, otherParam);
	}

	void NumericalOptimization::AddInequalityConstraint(ConstraintFunc func, void* otherParam /*= NULL*/, double tol /*= 0*/)
	{
		_opt->add_inequality_constraint(func, otherParam, tol);
	}

	void NumericalOptimization::AddEqualityConstraint(ConstraintFunc func, void* otherParam /*= NULL*/, double tol /*= 0*/)
	{
		_opt->add_equality_constraint(func, otherParam, tol);
	}

	void NumericalOptimization::SetUpperBound(const vector<double>& upper)
	{
		_opt->set_upper_bounds(upper);
	}

	void NumericalOptimization::SetUpperBound(const double& upper)
	{
		_opt->set_upper_bounds(upper);
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

	void NumericalOptimization::SetLowerBound(const vector<double>& lower)
	{
		_opt->set_lower_bounds(lower);
	}

	void NumericalOptimization::SetLowerBound(const double& lower)
	{
		_opt->set_lower_bounds(lower);
	}

}

