#include "stdafx.h"
#include "GrammarFeatureModel.h"
#include "GrammarFeatureDatabase.h"
#include "MindParameterDatabase.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/GrammarFeatureTemplate.h"
#include "../DataCollection/GrammarFeature.h"

#include "../Mathmatic/MathTool.h"
#include "../Mathmatic/NumericalOptimization.h"

#include <stdlib.h>

using namespace DataCollection;

namespace Mind
{
	GrammarFeatureTrainer::GrammarFeatureTrainer()
	{
		PrepareFeatureTemplates();
	}


	GrammarFeatureTrainer::~GrammarFeatureTrainer()
	{
	}

	void GrammarFeatureTrainer::CollectFeatures(const string samplePath)
	{
		auto allPOSsentences = ParseSampleSentences(samplePath);
		LOG_FORMAT("Finish parse sample sentences.The count of sentences is %d.", allPOSsentences.size());
		//Find features of sample sentences and features will be added to <_features>.
		FindFeatures(allPOSsentences);
		LOG_FORMAT("Finish find features of sample sentences and the count of features is %d.", _features.size());
		WriteFeaturesToDB();
		LOG("Finish write features to database.");
	}

	void GrammarFeatureTrainer::ComputeWeights(const string samplePath)
	{
		auto allPOSsentences = ParseSampleSentences(samplePath);

		GrammarFeatureModel featureModel;
		//Load all features from database for efficiency.
		featureModel.LoadAllFeatures();
		LOG("Load all features.");

		//test
		int i = 0;

		OptWeightParam optParam;
		set<string> featureTypes;//The count of types determines
		for (auto posSentence : allPOSsentences)
		{
			auto featureStat = featureModel.GetAllFeatures(posSentence);
			//Convert to map<string,int>.
			auto featureDistribution = ConvertToFeatureCountDistribution(featureStat);
			optParam.featureCounts.push_back(featureDistribution);
			//Collect feature types with no duplication.
			AddFeatureTypesForOpt(featureDistribution, featureTypes);
		}
		featureModel.ClearFeatures();
		//Add types.
		optParam.featureTypes.assign(featureTypes.begin(), featureTypes.end());
		LOG_FORMAT("There are %d types of features.", optParam.featureTypes.size());

		//Get random weights.
		vector<double> weights = Math::CreateRandomDoubleList(featureTypes.size());
		//Optimize.
		Math::NumericalOptimization opt(weights.size());
		opt.SetObjectiveFunction(OptFunc_ComputeWeights, &optParam);
		double objFunValue = 0;
		LOG_FORMAT("Initial objective function value is %lf.", ObjFunc(weights,&optParam));
		auto result = opt.Optimize(weights, objFunValue);
		LOG_FORMAT("Final objective function value is %lf.", objFunValue);
		//Write weights to database.
		MindParameterDatabase mindPramDB;
		mindPramDB.UpdateGrammarFeatureWeights(weights);
	}

	void GrammarFeatureTrainer::PrepareFeatureTemplates()
	{
		_featureTemplates.push_back(make_shared<TagWithWordTemplate>());
		_featureTemplates.push_back(make_shared<TagBigramTemplate>());
		_featureTemplates.push_back(make_shared<TagTrigramTemplate>());
		_featureTemplates.push_back(make_shared<TagFollowedByWordTemplate>());
		_featureTemplates.push_back(make_shared<WordFollowedByTagTemplate>());
	}

	vector<vector<shared_ptr<DataCollection::Word>>> GrammarFeatureTrainer::ParseSampleSentences(const string samplePath) const
	{
		vector<vector<shared_ptr<DataCollection::Word>>> res;
		ifstream in(samplePath, ios::binary);
		if (!in)
		{
			throw runtime_error("File not found: "+samplePath);
		}

		size_t index = 2;
		while (!in.eof())
		{
			//One line is raw sentence and the next line is POS sentence.

			//Raw sentence is not used.
			string raw = CommonTool::Getline_UnicodeFile(in, index);
			string POSUnsplit = CommonTool::Getline_UnicodeFile(in, index);

			//Parse the second line into pos sentence.
			try
			{
				auto onePosSentence = ParsePOSTagging(POSUnsplit);
				res.push_back(onePosSentence);
			}
			catch (const std::exception& ex)
			{
				LOG_EXCEPTION(ex);
			}
		}

		return res;
	}

	vector<shared_ptr<DataCollection::Word>> GrammarFeatureTrainer::ParsePOSTagging(const string line) const
	{
		//Split blank and get each word.
		auto split = CommonTool::SplitString(line, ' ');

		vector<shared_ptr<DataCollection::Word>> res;

		for (unsigned int i = 0; i < split.size(); ++i)
		{
			//Split '/' and get word string and pos.
			auto word_POS = CommonTool::SplitString(split[i], '/');
			if (word_POS.size() != 2)
			{
				throw runtime_error("Error in ParsePOSTagging");
			}

			res.push_back(LanguageFunc::GetParticularWord(word_POS[0], (PartOfSpeech)atoi(word_POS[1].c_str())));
		}

		return res;
	}

	void GrammarFeatureTrainer::FindFeatures(const vector<vector<shared_ptr<DataCollection::Word>>>& sentences) 
	{
		for (auto sentence : sentences)
		{
			FindFeaturesOneSentence(sentence);
		}
	}

	void GrammarFeatureTrainer::FindFeaturesOneSentence(const vector<shared_ptr<DataCollection::Word>>& sentence) 
	{
		for (auto featureTemplate : _featureTemplates)
		{
			//Each template will find many features in a sentence.
			auto features = featureTemplate->FindFeatures(sentence);
			for (auto feature : features)
			{
				//Check the hash of this feature.
				//If there is no such hash in the <_features>, then insert.
				size_t featureHash = feature->GetHash();
				if (_features.find(featureHash) == _features.end())
				{
					_features[featureHash] = feature;
				}
			}
		}
	}

	void GrammarFeatureTrainer::WriteFeaturesToDB() const
	{
		shared_ptr<GrammarFeatureDatabase> featureDB(new GrammarFeatureDatabase());
		vector<shared_ptr<GrammarFeature>> featureVec;
		for (auto feature : _features)
		{
			featureVec.push_back(feature.second);
		}
		featureDB->Insert(featureVec);
	}

	map<string, double> GrammarFeatureTrainer::ConvertToFeatureCountDistribution(const map<string, GrammarFeatureModel::StatList> &featureStat) const
	{
		map<string, int> freqDistri;
		double denominator = 0;//denominator of possibility.
		for (auto statPair : featureStat)
		{
			//Compute total count of a specific feature template.
			int countFeatureTemplate = 0;
			for (auto stat : statPair.second)
			{
				countFeatureTemplate += stat.count;
			}
			freqDistri[statPair.first] = countFeatureTemplate;
			denominator += exp(countFeatureTemplate);
		}

		//Convert frequency to possibility.
		map<string, double> res;
		for (auto freqPair : freqDistri)
		{
			double possi = exp(freqPair.second) / denominator;
			res[freqPair.first] = possi;
		}

		return res;
	}

	void GrammarFeatureTrainer::AddFeatureTypesForOpt(const map<string, double>& featureDistri, set<string>& types) const
	{
		for (auto pair : featureDistri)
		{
			types.insert(pair.first);
		}
	}

	double GrammarFeatureTrainer::OptFunc_ComputeWeights(const std::vector<double> &weights, std::vector<double> &grad, void* f_data)
	{
		OptWeightParam* param = reinterpret_cast<OptWeightParam*>(f_data);
		double obj = ObjFunc(weights, param);
		ComputeGrad(weights, param, grad);
		return obj;
	}

	double GrammarFeatureTrainer::ObjFunc(const vector<double>& weights, const OptWeightParam* param)
	{
		//Compute Objective function.
		double obj = 0;
		for (auto featureDistri : param->featureCounts)
		{
			double deviation = ComputeDeviation(weights, param->featureTypes, featureDistri);
			
			obj += deviation*deviation;
		}
		return obj;
	}

	void GrammarFeatureTrainer::ComputeGrad(const std::vector<double> &weights,const OptWeightParam* param, vector<double>& grad)
	{
		for (unsigned i = 0; i < param->featureTypes.size(); ++i)
		{
			//The ith feature type corresponds to the ith gradient.

			double gradient = 0;
			string featureType = param->featureTypes[i];
			//Compute each contribution of each sentence.
			for (auto featureDistri : param->featureCounts)
			{
				if (featureDistri.count(featureType))
				{
					double deviation = ComputeDeviation(weights, param->featureTypes, featureDistri);
					gradient += 2 * featureDistri[featureType] * deviation;
				}
			}
			grad[i] = gradient;
		}
	}

	double GrammarFeatureTrainer::ComputeDeviation(const vector<double>& weights, 
		const vector<string> &featureTypes, 
		const map<string, double> &featureDistri)
	{
		double deviation = -1;
		for (unsigned i = 0; i < featureTypes.size(); ++i)
		{
			string featureType = featureTypes[i];
			if (featureDistri.count(featureType))
			{
				deviation += featureDistri.at(featureType) * weights[i];
			}
		}
		return deviation;
	}

	GrammarFeatureModel::GrammarFeatureModel() :_featureDB(new GrammarFeatureDatabase())
	{
	}

	GrammarFeatureModel::~GrammarFeatureModel()
	{
		if (_featureDB != NULL)
		{
			delete _featureDB;
		}
	}

	double GrammarFeatureModel::ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return -1;
	}

	void GrammarFeatureModel::LoadAllFeatures()
	{
		//Get features from database.
		_featureDB->Connect();
		_features = _featureDB->GetAllFeatures();
		_featureDB->Disconnect();
	}

	map<string, GrammarFeatureModel::StatList> GrammarFeatureModel::GetAllFeatures(
		const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		//Find features in this sentence.
		map<string, GrammarFeatureModel::StatList> res;
		for (auto feature : _features)
		{
			int featureCount = feature->FeatureCount(sentence);
			if (featureCount > 0)
			{
				//Has feature in the sentence.

				FeatureStat stat;
				stat.feature = feature;
				stat.count = featureCount;
				string featureType = feature->GetMyType();
				if (res.count(featureType) == 0)
				{
					//A new type, then add StatList.
					res[featureType] = StatList();
				}
				res[featureType].push_back(stat);
			}
		}

		return res;
	}

}

