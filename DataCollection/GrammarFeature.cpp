#include "stdafx.h"
#include "GrammarFeature.h"
#include "LanguageFunc.h"

#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Character.h"

using namespace CommonTool;

namespace DataCollection
{

	map<string, FeatureCreator*> FeatureCreator::_featureCreators;

	bool FeatureCreator::_prepareTag= PrepareFeatureCreators();

	shared_ptr<GrammarFeature> FeatureCreator::CreateFeature(const CommonTool::DBRow& row)
	{
		if (_featureCreators.empty()) PrepareFeatureCreators();

		auto type = row.GetText("type");
		if (_featureCreators.count(type) == 0)
		{
			return NULL;
		}
		else
		{
			return _featureCreators[type]->Create(row);
		}
	}

	bool FeatureCreator::PrepareFeatureCreators()
	{
		HANDLE_ALL_FEATURES(AddFeatureCreator<, >());
		LOG("Grammar feature creators are prepared.");
		return true;
	}

	GrammarFeature::GrammarFeature()
	{

	}


	GrammarFeature::~GrammarFeature()
	{
	}

	shared_ptr<GrammarFeature> GrammarFeature::GetFeature(const CommonTool::DBRow& row)
	{
		return FeatureCreator::CreateFeature(row);
	}

	std::string GrammarFeature::GetMyType() const
	{
		return typeid(*this).name();
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
		string state = "Insert into GrammarFeature(id,type,pos1,pos2,pos3,word1,word2,word3)\
				VALUES (:id,:type,:pos1,:pos2,:pos3,:word1,:word2,:word3) ";
		DBCmd cmd(state, dbOpe);
		BindParam(cmd);
		//Set id.
		long long hash = GetHash()/2;
		cmd.Bind(":id", hash);
		//Set type of <me>.
		auto type = GetMyType();
		cmd.Bind(":type", type);

		return cmd;
	}


	void GrammarFeature::ReadFromDBRow(const CommonTool::DBRow& row)
	{
		ReadParam(row);
	}

	int TagWithWord::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		//Check if the string and POS are the same.
		auto curWord = words[i];
		if (GetString(0) == curWord->GetString() && GetPOS(0) == curWord->Type())
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
		if (GetPOS(0) == words[i - 1]->Type() && GetPOS(1) == words[i]->Type())
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

		if (GetPOS(0) == words[i - 1]->Type() && GetPOS(1) == words[i]->Type() && GetPOS(2) == words[i + 1]->Type())
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

		if (GetPOS(0) == words[i]->Type() && GetString(0) == words[i + 1]->GetString())
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

		if (GetString(0) == words[i]->GetString() && GetPOS(0) == words[i + 1]->Type())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int WordTagPreChar::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0) return 0;

		//Get last character of previous word.
		auto preWord = words[i - 1];
		auto lastChar = preWord->GetLastCharacter();

		if (GetString(0) == words[i]->GetString() && GetPOS(0) == words[i]->Type() && GetString(1) == lastChar.GetString())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int WordTagNextChar::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == words.size() - 1) return 0;

		//Get last character of previous word.
		auto nextWord = words[i + 1];
		auto nextChar = nextWord->GetLastCharacter();

		if (GetString(0) == words[i]->GetString() && GetPOS(0) == words[i]->Type() && GetString(1) == nextChar.GetString())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int SingleCharWithTrigramChar::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		if (i == 0 || i == words.size() - 1) return 0;

		auto preW = words[i - 1];
		auto nextW = words[i + 1];
		auto curW = words[i];
		if (preW->NumOfChara() == 1 && nextW->NumOfChara() == 1 && curW->NumOfChara() == 1)
		{
			if (curW->GetString() == GetString(0) &&curW->Type() == GetPOS(0) &&preW->GetString() == GetString(1) &&nextW->GetString() == GetString(2))
			{
				return true;
			}
		}

		return false;
	}

	int WordStartWithChar::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto curWord = words[i];
		if (GetString(0) == curWord->GetFirstCharacter().GetString() && GetPOS(0) == curWord->Type())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int WordEndWithChar::CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words)
	{
		auto curWord = words[i];
		if (GetString(0) == curWord->GetLastCharacter().GetString() && GetPOS(0) == curWord->Type())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}
