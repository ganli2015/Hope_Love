#include "stdafx.h"
#include "GrammarFeatureTemplate.h"
#include "GrammarFeature.h"

#include "../Mathmatic/Rand.h"

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

	std::string GrammarFeatureTemplate::PickRandomMiddleChar(const vector<Character>& characters) const
	{
		Math::Rand ran;
		int randIndex = ran.GetRandInt(1, characters.size() - 2);
		string midChar = characters[randIndex].GetString();

		return midChar;
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

	shared_ptr<GrammarFeature> WordTagPreCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0) return NULL;

		//Get last character of previous word.
		auto preWord = words[i - 1];
		auto lastChar = preWord->GetLastCharacter();

		return make_shared<WordTagPreChar>(words[i]->GetString(), words[i]->Type(), lastChar.GetString());
	}

	shared_ptr<GrammarFeature> WordTagNextCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == words.size() - 1) return NULL;

		//Get last character of previous word.
		auto nextWord = words[i + 1];
		auto nextChar = nextWord->GetLastCharacter();

		return make_shared<WordTagNextChar>(words[i]->GetString(), words[i]->Type(), nextChar.GetString());
	}


	shared_ptr<GrammarFeature> SingleCharWithTrigramCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0 || i == words.size() - 1) return NULL;

		auto preW = words[i - 1];
		auto nextW = words[i + 1];
		auto curW = words[i];
		if (preW->NumOfChara() == 1 && nextW->NumOfChara() == 1 && curW->NumOfChara() == 1)
		{
			return make_shared<SingleCharWithTrigramChar>(curW->GetString(), preW->GetString(), nextW->GetString(), curW->Type());
		}

		return NULL;
	}

	shared_ptr<GrammarFeature> WordStartWithCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		return make_shared<WordStartWithChar>(words[i]->GetFirstCharacter().GetString(), words[i]->Type());
	}

	shared_ptr<GrammarFeature> WordEndWithCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		return make_shared<WordEndWithChar>(words[i]->GetLastCharacter().GetString(), words[i]->Type());
	}

	shared_ptr<GrammarFeature> WordContainCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto characters = words[i]->GetCharatcters();
		if (characters.size() <= 2) return NULL;

		//Pick a random middle character.
		auto midChar = PickRandomMiddleChar(characters);
		return make_shared<WordContainChar>(midChar, words[i]->Type());
	}

	shared_ptr<GrammarFeature> WordContainCharStartWithCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto characters = words[i]->GetCharatcters();
		if (characters.size() <= 2) return NULL;

		auto midChar = PickRandomMiddleChar(characters);
		auto startChar = words[i]->GetFirstCharacter();

		return make_shared<WordContainCharStartWithChar>(midChar, startChar.GetString(), words[i]->Type());
	}

	shared_ptr<GrammarFeature> WordContainCharEndWithCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto characters = words[i]->GetCharatcters();
		if (characters.size() <= 2) return NULL;

		auto midChar = PickRandomMiddleChar(characters);
		auto lastChar = words[i]->GetLastCharacter();

		return make_shared<WordContainCharEndWithChar>(midChar, lastChar.GetString(), words[i]->Type());
	}

	shared_ptr<GrammarFeature> WordWithRepeatedCharTemplate::FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto curWord = words[i];
		auto characters = curWord->GetCharatcters();

		for (int j = 0; j < characters.size() - 1; ++j)
		{
			if (characters[j].GetString() ==  characters[j + 1].GetString())
			{
				return make_shared<WordWithRepeatedChar>(characters[j].GetString(), curWord->Type());;
			}
		}

		return NULL;
	}

}

