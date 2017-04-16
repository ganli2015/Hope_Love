#pragma once
#include "GrammarModel.h"
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

	class _MINDINOUT GrammarFeatureModel : public GrammarModel
	{
		//////////////////////////////////////////////////////////////////////////
		//The order of <_featureTypes> determines the order of weights in the database! 
		//////////////////////////////////////////////////////////////////////////
		static set<string> _featureTypes;

		GrammarFeatureDatabase *_featureDB;
		FeatureList _features;
		bool _loadedFeatures;

		//////////////////////////////////////////////////////////////////////////
		//Key is feature type.
		//////////////////////////////////////////////////////////////////////////
		map<string, double> _weights;

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
		virtual ~GrammarFeatureModel();

		static size_t GetFeatureTypesCount() { return _featureTypes.size(); }

		//////////////////////////////////////////////////////////////////////////
		//Compute Possibility of tagging for a given sentence. 
		//Call LoadAllFeatures before computing!!
		//////////////////////////////////////////////////////////////////////////
		double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const;

		//////////////////////////////////////////////////////////////////////////
		//Load all features from database to memory.
		//////////////////////////////////////////////////////////////////////////
		void LoadAllFeatures();
		void ClearFeatures() { _features.clear(); _loadedFeatures = false; }
		bool FeaturesLoaded() { return _loadedFeatures; }

	private:
		template<typename T>
		static void AddFeature(set<string>& features)
		{
			features.insert(make_shared<T>()->GetMyType());
		}

		static set<string> PrepareGrammarFeatures();

		void ReadWeightsInDB();
		

		//////////////////////////////////////////////////////////////////////////
		//Get features of sentence.
		//Return a map: key is feature type and value is corresponding features.
		//////////////////////////////////////////////////////////////////////////
		map<string, StatList> GetAllFeatures(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
		map<string, double> ConvertToFeatureCountDistribution(const map<string, GrammarFeatureModel::StatList> &featureStat) const;
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
		template<typename T>
		void AddFeatureTemplate()
		{
			_featureTemplates.push_back(make_shared<T>());
		}

		void FindFeatures(const vector<vector<shared_ptr<DataCollection::Word>>>& sentences) ;
		void FindFeaturesOneSentence(const vector<shared_ptr<DataCollection::Word>>& sentence) ;

		void WriteFeaturesToDB() const;

		static double OptFunc_ComputeWeights(const std::vector<double> &weights,
			std::vector<double> &grad,
			void* f_data);
		//////////////////////////////////////////////////////////////////////////
		//The constraint on last weight that must be 0 to 1.
		//////////////////////////////////////////////////////////////////////////
		static double LastWeightConstraint(const vector<double>& weights, std::vector<double> &grad,
			void* f_data);

		static double ObjFunc(const vector<double>& weights,const OptWeightParam* param);
		static void ComputeGrad(const std::vector<double> &weights, const OptWeightParam* param, vector<double>& grad);
		//////////////////////////////////////////////////////////////////////////
		//Compute difference between a sentence grammar possibility and one.
		//////////////////////////////////////////////////////////////////////////
		static double ComputeObjComponent(const vector<double>& weights, const vector<string> &featureTypes, const map<string, double> &featureDistri);
	
		static double ComputeLastWeight(const vector<double>& weights);
	};


	
}


