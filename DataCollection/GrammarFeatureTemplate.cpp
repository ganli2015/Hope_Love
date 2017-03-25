#include "stdafx.h"
#include "GrammarFeatureTemplate.h"
#include "GrammarFeature.h"


namespace DataCollection
{
	GrammarFeatureTemplate::GrammarFeatureTemplate()
	{
	}


	GrammarFeatureTemplate::~GrammarFeatureTemplate()
	{
	}


	vector<shared_ptr<GrammarFeature>> GrammarFeatureTemplate::FindFeatures(const vector<shared_ptr<Word>>& words)
	{
		vector<shared_ptr<GrammarFeature>> res;
		for (size_t i = 0; i < words.size(); ++i)
		{
			auto feature = FindOneFeature(i, words);
			//If find no feature, return null.
			if(feature!=NULL)
				res.push_back(feature);
		}

		return res;
	}

	TagWithWordTemplate::TagWithWordTemplate()
	{
	}

	TagWithWordTemplate::~TagWithWordTemplate()
	{
	}

	shared_ptr<GrammarFeature> TagWithWordTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		shared_ptr<TagWithWord> feature(new TagWithWord(words[i]));
		return feature;
	}

	shared_ptr<GrammarFeature> TagBigramTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0) return NULL;

		//Get current POS and the previous one.
		shared_ptr<TagBigram> feature(new TagBigram(words[i - 1]->Type(), words[i]->Type()));
		return feature;
	}

	shared_ptr<GrammarFeature> TagTrigramTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0 || i == words.size() - 1)
		{
			return NULL;
		}

		shared_ptr<GrammarFeature> feature(new TagTrigram(words[i - 1]->Type(), words[i]->Type(), words[i + 1]->Type()));
		return feature;
	}

	shared_ptr<GrammarFeature> TagFollowedByWordTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == words.size() - 1) return NULL;

		shared_ptr<GrammarFeature> feature(new TagFollowedByWord(words[i]->Type(), words[i + 1]->GetString()));
		return feature;
	}

	shared_ptr<GrammarFeature> WordFollowedByTagTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == words.size() - 1) return NULL;

		shared_ptr<GrammarFeature> feature(new WordFollowedByTag(words[i]->GetString(), words[i + 1]->Type()));
		return feature;
	}

}

