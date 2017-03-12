#include "StdAfx.h"
#include "MultilayerNetworkTrainImp.h"

#include "../CommonTools/LogWriter.h"

#include <fstream>

namespace NeuralNetwork
{
	MultilayerNetworkTrainImp::MultilayerNetworkTrainImp(void):_out("DebugInfo//Network Object Function.txt")
	{
	}


	MultilayerNetworkTrainImp::~MultilayerNetworkTrainImp(void)
	{
	}

	void MultilayerNetworkTrainImp::OutputDebugInfo() 
	{
		DEBUG_FORMAT("The object function value is %f", _obj);
	}

}

