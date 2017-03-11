#include "StdAfx.h"
#include "ConceptReactSystem.h"
#include "ConceptSet.h"
#include "FilePath.h"
#include "Cerebrum.h"
#include "CommonFunction.h"
#include "ConceptReactImp.h"
#include "ConceptReactImp_1234.h"
#include "ConceptReactImp_1212.h"
#include "ReactionValueEvaluation.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/ConfigureInfoManager.h"
#include "../CommonTools/LogWriter.h"

#include "../Mathmatic/Rand.h"
#include "../Mathmatic/FindSequence.h"
#include "../Mathmatic/iMatrix.h"
#include "../Mathmatic/Vector.h"

#include "../Neural Network Design/DataArray.h"
#include "../Neural Network Design/Neuron.h"
#include "../Neural Network Design/TransferFunctions.h"
#include "../Neural Network Design/MultilayerNetwork.h"
#include "../Neural Network Design/ADALINE.h"
#include "../Neural Network Design/Errors.h"

#include "../MindInterface/iConcept.h"

#include "../MindElement/ConceptChain.h"
#include "../MindElement/BaseConcept.h"

#include <numeric>


using namespace Math;
using namespace NeuralNetwork;
namespace Mind
{
	ConceptReactSystem::ConceptReactSystem(ConceptSet* val) :ioTag('&'), conceptTag('#'), idStrTag(' '), _conceptSet(val)
	{
		Initialize();
		LOG("Initialized ConceptReactSystem.");
	}

	ConceptReactSystem::~ConceptReactSystem(void)
	{
	}

	void ConceptReactSystem::Initialize()
	{
		//ReactionValueEvaluation valueEval;

		//Train network or initialize network from file according to the configure file.
		CFG_IF(Train_Initial_React_Network,
			BasicTrainNetwork();,
			InitNetworkFromFile();
		);
	}


	void ConceptReactSystem::InitNetworkFromFile()
	{
		LOG("Read network from file.");
		int dimension = _conceptSet->BaseConceptCount();
		LOG_FORMAT("Neuron dimension: %d", dimension);

		//Check whether the file exists.
		//If not retrain network, otherwise read data from file.
		//Check whether the count of base concepts equals to the input dimension.
		//If not, network cannot be initialized as lack of neuron data.
		ifstream in(GetHopeLoveMindPath() + ConceptReactorNetworkFilename);
		if (!in || !SameInputDimension(dimension, in))
		{
			BasicTrainNetwork();
			return;
		}
		else
		{
			in.close();
		}

		//The dimension of network should be designated first.
		shared_ptr<MultilayerNetwork> multiNetwork(new MultilayerNetwork(dimension, dimension));
		_network = multiNetwork;
		_network->Read(GetHopeLoveMindPath() + ConceptReactorNetworkFilename);
	}

	bool ConceptReactSystem::SameInputDimension(const int baseConceptCount, ifstream& in)
	{
		//The input dimension in the file is the seventh element.
		//Read from beginning of the file and skip previous parameters.
		int inputDimIndex = 7;
		int curIndex = 0;
		//The type of input parameter is double 
		//as there is a parameter of 1e-6 to read.
		double para;
		while (++curIndex <= 7)
		{
			in >> para;
		}

		if (baseConceptCount == round(para))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ConceptReactSystem::BasicTrainNetwork()
	{
		ifstream in(GetHopeLoveMindPath() + ConceptReactor_InitialFilename);
		if (!in) return;

		//Read concept react data from file.
		vector<DataInfo> dataInfos;
		string str;
		while (getline(in, str))
		{
			vector<string> i_o = CommonTool::SplitString(str, ioTag);
			if (i_o.size() != 2)
			{
				throw logic_error("Error in ConceptReactSystem::Initialize");
			}

			shared_ptr<ConceptChain> chain_i = ParseChain(i_o[0]);
			shared_ptr<ConceptChain> chain_o = ParseChain(i_o[1]);
			DataInfo info;
			info.input = chain_i;
			info.expect = chain_o;
			dataInfos.push_back(info);
		}

		LOG("Begin train reaction network.");
		try
		{
			Train(dataInfos);
			LOG("Finish train reaction network.");
			_network->Write(GetHopeLoveMindPath() + ConceptReactorNetworkFilename);
		}
		catch (const std::exception& ex)
		{
			LOG_EXCEPTION(ex);
		}
	}

	shared_ptr<ConceptChain> ConceptReactSystem::ParseChain(const string str) const
	{
		vector<shared_ptr<iConcept>> conceptVec;
		//解析为多个Identity
		vector<string> conceptStr = CommonTool::SplitString(str, conceptTag);
		for (unsigned int i = 0; i < conceptStr.size(); ++i)
		{
			//解析每个Identity
			vector<string> split = CommonTool::SplitString(conceptStr[i], idStrTag);

			if (split.size() != 2)
			{
				throw logic_error("Error in ParseChain");
			}

			shared_ptr<iConcept> concept = _conceptSet->GetConceptPtr(CommonFunction::TransformToIdentity(split[0], split[1]));
			DEBUG_IF(concept == NULL, "Fail to get concept.", split[1]);

			conceptVec.push_back(concept);
		}

		return shared_ptr<ConceptChain>(new ConceptChain(conceptVec));
	}

	void ConceptReactSystem::Train(const vector<DataInfo>& dataInfos)
	{
		if (dataInfos.empty()) return;

		int dimension = _conceptSet->BaseConceptCount();
		LOG_FORMAT("Neuron dimension: %d" , dimension);
		shared_ptr<iNeuron> neu1 = InitNeuron(dimension, dimension);
		LOG("Neuron initialized.");
		//		shared_ptr<iNeuron> neu2=InitNeuron(interDim,dimension);

		// 		int interDim=50;
		// 		shared_ptr<iNeuron> neu1(new Neuron(dimension,interDim));
		// 		neu1->SetBias(Vector(interDim,0));
		// 		shared_ptr<TransferFunction::fun> fun_logsig(new TransferFunction::purelin);
		// 		neu1->SetFun(fun_logsig);
		// 		shared_ptr<iNeuron> neu2(new Neuron(interDim,dimension));
		// 		neu2->SetBias(Vector(dimension,0));
		// 		neu2->SetFun(fun_logsig);

		shared_ptr<MultilayerNetwork> multiNetwork(new MultilayerNetwork(dimension, dimension));
		multiNetwork->SetMyNeuron(0, neu1);
		//		multiNetwork->SetMyNeuron(1,neu2);

		for (unsigned int i = 0; i < dataInfos.size(); ++i)
		{
			shared_ptr<iDataArray> proto = CommonFunction::ToDataArray(dataInfos[i].input, _conceptSet);
			shared_ptr<iDataArray> expect = CommonFunction::ToDataArray(dataInfos[i].expect, _conceptSet);
			multiNetwork->SetMyData(proto, expect);
		}
		multiNetwork->SetLearningRate(0.9);
		multiNetwork->SetMaxIterationCount(200);

		TrainResult result = multiNetwork->Training();

		if (result != Success)
		{
			vector<double> residuals;
			for (unsigned int i = 0; i < dataInfos.size(); ++i)
			{
				residuals.push_back(ComputeStandardDeviation(dataInfos[i], multiNetwork));
			}
			double objFun = accumulate(residuals.begin(), residuals.end(), 0.);
		}

		_network = multiNetwork;
	}

	shared_ptr<iNeuron> ConceptReactSystem::InitNeuron(const int i, const int j)
	{
		double randDouble = Rand::GetRandDecimal();

		shared_ptr<iNeuron> neu(new Neuron(CreateRandomNeuronMatrix(i, j)));

		shared_ptr<TransferFunction::fun> transferFun(new TransferFunction::purelin);
		neu->SetFun(transferFun);

		return neu;
	}

	Math::Matrix ConceptReactSystem::CreateRandomNeuronMatrix(const int i, const int j)
	{
		//If i and j are too large, then divide them into small pieces.
		//Assign one random value to one piece and then construct all pieces to the large matrix.

		//Generate a list of random double.
		const unsigned int randomSize = 1000;
		vector<double> randomList(randomSize);
		for (unsigned int i = 0; i < randomSize; ++i)
		{
			randomList[i] = Rand::GetRandDecimal();
		}
		LOG_FORMAT("Generate %d random doubles for initialize matrix.", randomSize);

		//Generate random matrix.
		Matrix res(i, j);
		int randIndex = 0;
		for (int n = 0; n < j; ++n)
		{
			Vector vec(i);
			for (int m = 0; m < i; ++m)
			{
				double val = randomList[randIndex];
				vec.Set_ithVal(m, val);

				//Increase index of random list.
				if (++randIndex >= randomSize)
				{
					randIndex = 0;
				}
			}
			vec = vec.Normalized();
			res.Set_jthColumn(n, vec);
		}

		return res;
	}

	double ConceptReactSystem::ComputeStandardDeviation(const DataInfo& data, shared_ptr<Network> network)
	{
		shared_ptr<iDataArray> output = network->GetResult(CommonFunction::ToDataArray(data.input, _conceptSet));
		shared_ptr<iDataArray> residual = output->Subtract(CommonFunction::ToDataArray(data.expect, _conceptSet));
		return residual->Norm();
	}

	vector<ConceptChainProperty> ConceptReactSystem::React(const shared_ptr<iConceptChain> chain)
	{
		shared_ptr<ConceptReactImp> reactImp(new ConceptReactImp_1212(_network, _conceptSet));
		return reactImp->Perform(chain);
	}

	void ConceptReactSystem::NormalizeConfidence(vector<ConceptChainProperty>& vec)
	{
		vector<double> dev_unnormalized;
		dev_unnormalized.reserve(vec.size());
		for (unsigned int i = 0; i < vec.size(); ++i)
		{
			dev_unnormalized.push_back(vec[i].confidence);
		}

		vector<double> dev_normalized = Normalized(dev_unnormalized);
		for (unsigned int i = 0; i < dev_normalized.size(); ++i)
		{
			vec[i].confidence = dev_normalized[i];
		}
	}



}
