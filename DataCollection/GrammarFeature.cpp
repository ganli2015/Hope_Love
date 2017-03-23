#include "stdafx.h"
#include "GrammarFeature.h"


namespace DataCollection
{
	GrammarFeature::GrammarFeature()
	{
	}


	GrammarFeature::~GrammarFeature()
	{
	}


	int GrammarFeature::FeatureCount(const vector<shared_ptr<Word>>& words)
	{
		int res = 0;
		for (size_t i = 0; i < words.size(); ++i)
		{
			auto currentCount = CurrentFeatureCount(i, words);
			res += currentCount;
		}

		return res;
	}

	TagWithWord::TagWithWord(const shared_ptr<Word> val) :_word(val)
	{
	}

	TagWithWord::~TagWithWord()
	{
	}

	int TagWithWord::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check if the string and POS are the same.
		auto curWord = words[i];
		if (curWord->IsSame(_word))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int TagBigram::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check POS of current and previous word.
		if (i == 0) return 0;
		if (_t1 == words[i - 1]->Type() && _t2 == words[i]->Type())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int TagTrigram::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check POS of current,previous and next word.
		if (i == 0 || i == words.size() - 1) return 0;

		if (_t1 == words[i - 1]->Type() && _t2 == words[i]->Type() && _t3 == words[i + 1]->Type())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int TagFollowedByWord::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check current POS and next word string.
		if (i == words.size() - 1) return 0;

		if (_t1 == words[i]->Type() && _word->GetString() == words[i + 1]->GetString())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int WordFollowedByTag::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check current word and next POS.
		if (i == words.size() - 1) return 0;

		if (_word->GetString() == words[i]->GetString() && _t1 == words[i + 1]->Type())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

}
