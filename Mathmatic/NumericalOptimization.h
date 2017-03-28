#pragma once
#include "InOut.h"
#include "functional"

struct nlopt_opt_s; /* opaque structure, defined internally */
typedef struct nlopt_opt_s *nlopt_opt;
namespace Mind
{
	class _MATHMATICINOUT NumericalOptimization
	{
		nlopt_opt *_opt;
	public:
		NumericalOptimization();
		~NumericalOptimization();

		void SetObjectiveFunction(function<double(unsigned, const double*, double, void*)> objFunc);

	private:
		//////////////////////////////////////////////////////////////////////////
		//Forbid copy!
		//////////////////////////////////////////////////////////////////////////
		NumericalOptimization(const NumericalOptimization& other);
	};
}


