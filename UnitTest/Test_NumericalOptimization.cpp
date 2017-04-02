#include "stdafx.h"
#include "Test_NumericalOptimization.h"

#include "../Mathmatic/NumericalOptimization.h"

using namespace Math;

const static double tolerance = 1e-4;

namespace NonlinearlyConstrainedProblem
{
	typedef struct {
		double a, b;
	} my_constraint_data;

	my_constraint_data data[2] = { { 2,0 },{ -1,1 } };

	double myfunc(const vector<double> &x, vector<double> &grad, void *my_func_data)
	{
		if (!grad.empty()) {
			grad[0] = 0.0;
			grad[1] = 0.5 / sqrt(x[1]);
		}
		return sqrt(x[1]);
	}

	double myconstraint(const vector<double> &x, vector<double> &grad, void *my_func_data)
	{
		my_constraint_data *d = (my_constraint_data *)my_func_data;
		double a = d->a, b = d->b;
		if (!grad.empty()) {
			grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
			grad[1] = -1.0;
		}
		return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
	}
}


//The example are copied from http://ab-initio.mit.edu/wiki/index.php/NLopt_Tutorial.
TEST(Test_NumericalOptimization, NonlinearlyConstrainedProblem)
{
	vector<double> lb{ -HUGE_VAL, 0 };

	NumericalOptimization opt( 2, Math::LD_MMA);
	opt.SetLowerBound(lb);
	opt.SetObjectiveFunction(NonlinearlyConstrainedProblem::myfunc);
	opt.SetInequalityConstraint(NonlinearlyConstrainedProblem::myconstraint, &NonlinearlyConstrainedProblem::data[0], 1e-8);
	opt.SetInequalityConstraint(NonlinearlyConstrainedProblem::myconstraint, &NonlinearlyConstrainedProblem::data[1], 1e-8);
	opt.SetXTol(1e-8);

	vector<double> x{ 1.234, 5.678 };
	double funVal;
	OptResult result = opt.Optimize(x, funVal);

	ASSERT_NEAR(0.333334, x[0], tolerance);
	ASSERT_NEAR(0.296296, x[1], tolerance);
	ASSERT_NEAR(0.544330847, funVal, tolerance);
}