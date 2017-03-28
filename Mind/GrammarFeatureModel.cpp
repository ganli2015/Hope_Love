#include "stdafx.h"
#include "GrammarFeatureModel.h"
#include "GrammarFeatureDatabase.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/GrammarFeatureTemplate.h"
#include "../DataCollection/GrammarFeature.h"

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

	GrammarFeatureModel::GrammarFeatureModel()
	{
	}

	GrammarFeatureModel::~GrammarFeatureModel()
	{
	}

	double GrammarFeatureModel::ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return -1;
	}

}

