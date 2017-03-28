#pragma once
#include "InOut.h"



namespace DataCollection
{
	class Word;
	class GrammarFeature;
	class GrammarFeatureTemplate;
}

namespace Mind
{
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
	};

	class GrammarFeatureModel
	{
	public:
		GrammarFeatureModel();
		~GrammarFeatureModel();

		//////////////////////////////////////////////////////////////////////////
		//Compute Possibility of tagging for a given sentence. 
		//////////////////////////////////////////////////////////////////////////
		double ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
	private:

	};


}


