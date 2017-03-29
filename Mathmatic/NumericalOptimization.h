#pragma once
#include "InOut.h"
#include "functional"

namespace nlopt
{
	class opt;
}

namespace Math
{
	enum algorithm {
		GN_DIRECT = 0,
		GN_DIRECT_L,
		GN_DIRECT_L_RAND,
		GN_DIRECT_NOSCAL,
		GN_DIRECT_L_NOSCAL,
		GN_DIRECT_L_RAND_NOSCAL,
		GN_ORIG_DIRECT,
		GN_ORIG_DIRECT_L,
		GD_STOGO,
		GD_STOGO_RAND,
		LD_LBFGS_NOCEDAL,
		LD_LBFGS,
		LN_PRAXIS,
		LD_VAR1,
		LD_VAR2,
		LD_TNEWTON,
		LD_TNEWTON_RESTART,
		LD_TNEWTON_PRECOND,
		LD_TNEWTON_PRECOND_RESTART,
		GN_CRS2_LM,
		GN_MLSL,
		GD_MLSL,
		GN_MLSL_LDS,
		GD_MLSL_LDS,
		LD_MMA,
		LN_COBYLA,
		LN_NEWUOA,
		LN_NEWUOA_BOUND,
		LN_NELDERMEAD,
		LN_SBPLX,
		LN_AUGLAG,
		LD_AUGLAG,
		LN_AUGLAG_EQ,
		LD_AUGLAG_EQ,
		LN_BOBYQA,
		GN_ISRES,
		AUGLAG,
		AUGLAG_EQ,
		G_MLSL,
		G_MLSL_LDS,
		LD_SLSQP,
		LD_CCSAQ,
		GN_ESCH,
		NUM_ALGORITHMS /* not an algorithm, just the number of them */
	};

	enum OptResult
	{
		FAILURE = -1, /* generic failure code */
		INVALID_ARGS = -2,
		OUT_OF_MEMORY = -3,
		ROUNDOFF_LIMITED = -4,
		FORCED_STOP = -5,
		SUCCESS = 1, /* generic success code */
		STOPVAL_REACHED = 2,
		FTOL_REACHED = 3,
		XTOL_REACHED = 4,
		MAXEVAL_REACHED = 5,
		MAXTIME_REACHED = 6
	};

	//////////////////////////////////////////////////////////////////////////
	//Object function expression.
	//n: number of variables.
	//x: variables.
	//gradient: gradients of variables.
	//func_data: other parameters.
	//////////////////////////////////////////////////////////////////////////
	typedef double(*ObjectFunction)(const std::vector<double> &x, 
		std::vector<double> &grad, 
		void* f_data);

	typedef ObjectFunction ConstraintFunc;

	class _MATHMATICINOUT NumericalOptimization
	{
		nlopt::opt *_opt;
		vector<double> _upperBound;
		vector<double> _lowerBound;

	public:
		NumericalOptimization(const algorithm alg,const unsigned n);
		~NumericalOptimization();

		void SetObjectiveFunction(ObjectFunction objFunc,void* otherParam=NULL);
		void SetInequalityConstraint(ConstraintFunc func, void* otherParam = NULL, double tol = 0);
		void SetEqualityConstraint(ConstraintFunc func, void* otherParam = NULL, double tol = 0);

		void SetUpperBound(const vector<double>& upper) { _upperBound = upper; };
		void SetLowerBound(const vector<double>& lower) { _lowerBound = lower; };

		void SetXTol(const double xtol);
		void SetFTol(const double ftol);
		void SetMaxIteration(const unsigned maxIter);

		OptResult Optimize(std::vector<double> &x, double &opt_f);

	private:
		//////////////////////////////////////////////////////////////////////////
		//Forbid copy!
		//////////////////////////////////////////////////////////////////////////
		NumericalOptimization(const NumericalOptimization& other);
	};
}


