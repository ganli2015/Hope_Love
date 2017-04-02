#pragma once
#include "InOut.h"
#include <set>


namespace DataCollection
{
	class Word;
	class GrammarFeature;
	class GrammarFeatureTemplate;
}

namespace Mind
{
	class GrammarFeatureDatabase;

	class _MINDINOUT GrammarFeatureModel
	{
		GrammarFeatureDatabase *_featureDB;
		FeatureList _features;

		friend class GrammarFeatureTrainer;
	private:
		struct FeatureStat
		{
			shared_ptr<DataCollection::GrammarFeature> feature;
			int count;//Satisfied feature count in a sentence.
		};

		typedef vector<FeatureStat> StatList;

	public:
		GrammarFeatureModel();
		~GrammarFeatureModel();

		//////////////////////////////////////////////////////////////////////////
		//Compute Possibility of tagging for a given sentence. 
		//////////////////////////////////////////////////////////////////////////
		double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const;

		//////////////////////////////////////////////////////////////////////////
		//Load all features from database to memory.
		//////////////////////////////////////////////////////////////////////////
		void LoadAllFeatures();
		void ClearFeatures() { _features.clear(); }

	private:
		//////////////////////////////////////////////////////////////////////////
		//Get features of sentence.
		//Return a map: key is feature type and value is corresponding features.
		//////////////////////////////////////////////////////////////////////////
		map<string, StatList> GetAllFeatures(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
	};

	//////////////////////////////////////////////////////////////////////////
	//Do some training or statistical techniques with grammar feature.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINOUT GrammarFeatureTrainer
	{
		//////////////////////////////////////////////////////////////////////////
		//Key is hash of a feature.
		//////////////////////////////////////////////////////////////////////////
		map<size_t, shared_ptr<DataCollection::GrammarFeature>> _features;

		vector<shared_ptr<DataCollection::GrammarFeatureTemplate>> _featureTemplates;

		friend class GrammarFeatureModel;

	private:
		//////////////////////////////////////////////////////////////////////////
		//Parameters for optimizing weights.
		//////////////////////////////////////////////////////////////////////////
		struct OptWeightParam
		{
			//////////////////////////////////////////////////////////////////////////
			//Each element is feature distribution of each sentence.
			//In map<string, int>, key is feature type and value is its count in this sentence.
			//////////////////////////////////////////////////////////////////////////
			vector<map<string, double>> featureCounts;

			//////////////////////////////////////////////////////////////////////////
			//Sequence of feature types.Its order is the same as variable.
			//////////////////////////////////////////////////////////////////////////
			vector<string> featureTypes;
		};

	public:
		GrammarFeatureTrainer();
		~GrammarFeatureTrainer();

		//////////////////////////////////////////////////////////////////////////
		//Read and collect grammar features from the file <samplePath>.
		//The features will write to the database (..\Mind\HopeLoveData\data.db).
		//////////////////////////////////////////////////////////////////////////
		void CollectFeatures(const string samplePath);

		//////////////////////////////////////////////////////////////////////////
		//Compute weights for different features.
		//////////////////////////////////////////////////////////////////////////
		void ComputeWeights(const string samplePath);

	private:
		void PrepareFeatureTemplates();

		vector<vector<shared_ptr<DataCollection::Word>>> ParseSampleSentences(const string samplePath) const;
		vector<shared_ptr<DataCollection::Word>> ParsePOSTagging(const string line) const;

		void FindFeatures(const vector<vector<shared_ptr<DataCollection::Word>>>& sentences) ;
		void FindFeaturesOneSentence(const vector<shared_ptr<DataCollection::Word>>& sentence) ;

		void WriteFeaturesToDB() const;

		map<string, double> ConvertToFeatureCountDistribution(const map<string, GrammarFeatureModel::StatList> &featureStat) const;
		void AddFeatureTypesForOpt(const map<string, double>& featureDistri, set<string>& types) const;

		static double OptFunc_ComputeWeights(const std::vector<double> &weights,
			std::vector<double> &grad,
			void* f_data);

		static double ObjFunc(const vector<double>& weights,const OptWeightParam* param);
		static void ComputeGrad(const std::vector<double> &weights, const OptWeightParam* param, vector<double>& grad);
		//////////////////////////////////////////////////////////////////////////
		//Compute difference between a sentence grammar possibility and one.
		//////////////////////////////////////////////////////////////////////////
		static double ComputeDeviation(const vector<double>& weights, const vector<string> &featureTypes, const map<string, double> &featureDistri);
	};


	
}


