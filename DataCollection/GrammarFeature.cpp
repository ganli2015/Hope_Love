#include "stdafx.h"
#include "GrammarFeature.h"
#include "LanguageFunc.h"

#include "../CommonTools/DBoperator.h"

using namespace CommonTool;

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

	CommonTool::DBCmd GrammarFeature::GetInsertCmd(CommonTool::DBoperator& dbOpe) const
	{
		string state = "Insert into GrammarFeature(freq,type,pos1,pos2,pos3,word1,word2,word3)\
				VALUES (:freq,:type,:pos1,:pos2,:pos3,:word1,:word2,:word3) ";
		DBCmd cmd(state, dbOpe);
		BindParam(cmd);

		return cmd;
	}


	void GrammarFeature::ReadFromDBRow(const CommonTool::DBRow& row)
	{
		ReadParam(row);
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

	void TagWithWord::BindParam(CommonTool::DBCmd& cmd) const
	{
		cmd.Bind(":pos1", (int)_word->Type());
		cmd.Bind(":word1", _word->GetString());
	}

	void TagWithWord::ReadParam(const CommonTool::DBRow& row)
	{
		auto pos = (PartOfSpeech)row.GetLong("pos1");
		string wordStr = row.GetText("word1");
		_word = LanguageFunc::GetParticularWord(wordStr, pos);
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

	void TagBigram::BindParam(CommonTool::DBCmd& cmd) const
	{
		cmd.Bind(":pos1", (int)_t1);
		cmd.Bind(":pos2", (int)_t2);
	}

	void TagBigram::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_t2 = (PartOfSpeech)row.GetLong("pos2");
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

	void TagTrigram::BindParam(CommonTool::DBCmd& cmd) const
	{
		cmd.Bind(":pos1", (int)_t1);
		cmd.Bind(":pos2", (int)_t2);
		cmd.Bind(":pos3", (int)_t3);
	}

	void TagTrigram::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_t2 = (PartOfSpeech)row.GetLong("pos2");
		_t3 = (PartOfSpeech)row.GetLong("pos3");
	}

	int TagFollowedByWord::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check current POS and next word string.
		if (i == words.size() - 1) return 0;

		if (_t1 == words[i]->Type() && _word == words[i + 1]->GetString())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void TagFollowedByWord::BindParam(CommonTool::DBCmd& cmd) const
	{
		cmd.Bind(":pos1", (int)_t1);
		cmd.Bind(":word1", _word);
	}

	void TagFollowedByWord::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_word = row.GetText("word1");
	}

	int WordFollowedByTag::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check current word and next POS.
		if (i == words.size() - 1) return 0;

		if (_word == words[i]->GetString() && _t1 == words[i + 1]->Type())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void WordFollowedByTag::BindParam(CommonTool::DBCmd& cmd) const
	{
		cmd.Bind(":pos1", (int)_t1);
		cmd.Bind(":word1", _word);
	}

	void WordFollowedByTag::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_word = row.GetText("word1");
	}

}
