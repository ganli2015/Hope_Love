#include "stdafx.h"
#include "GrammarFeature.h"
#include "LanguageFunc.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/LogWriter.h"

using namespace CommonTool;

namespace DataCollection
{

	map<string, GrammarFeature::FeatureCreator*> GrammarFeature::_featureCreators;

	GrammarFeature::GrammarFeature()
	{
		if (_featureCreators.empty())
		{
			PrepareFeatureCreators();
		}
	}


	GrammarFeature::~GrammarFeature()
	{
	}

	shared_ptr<GrammarFeature> GrammarFeature::GetFeature(const CommonTool::DBRow& row)
	{
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

	void GrammarFeature::PrepareFeatureCreators()
	{
		string typePrefix = "class DataCollection::";

		_featureCreators[typePrefix+"TagWithWord"] = new ConcreteFeatureCreator<TagWithWord>();
		_featureCreators[typePrefix+"TagBigram"] = new ConcreteFeatureCreator<TagBigram>();
		_featureCreators[typePrefix+"TagTrigram"] = new ConcreteFeatureCreator<TagTrigram>();
		_featureCreators[typePrefix+"TagFollowedByWord"] = new ConcreteFeatureCreator<TagFollowedByWord>();
		_featureCreators[typePrefix+"WordFollowedByTag"] = new ConcreteFeatureCreator<WordFollowedByTag>();
		LOG("Grammar feature creators are prepared.");
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

	TagWithWord::TagWithWord(const shared_ptr<Word> val) :_word(val)
	{
	}

	TagWithWord::~TagWithWord()
	{
	}

	bool TagWithWord::Same(const shared_ptr<GrammarFeature> other) const
	{
		//Check type.
		auto otherDrived = dynamic_pointer_cast<TagWithWord>(other);
		if (otherDrived == NULL) return false;

		if (_word->IsSame(otherDrived->_word))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t TagWithWord::GetHash() const
	{
		string hashStr = GetMyType() + _word->GetString() + ToString(_word->Type());
		return GetStrHash(hashStr);
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
		cmd.Bind(":word1", _word->GetString(Word::Utf8));
	}

	void TagWithWord::ReadParam(const CommonTool::DBRow& row)
	{
		auto pos = (PartOfSpeech)row.GetLong("pos1");
		string wordStr = row.GetText("word1");
		_word = LanguageFunc::GetParticularWord(wordStr, pos);
	}

	bool TagBigram::Same(const shared_ptr<GrammarFeature> other) const
	{
		//Check type.
		auto otherDrived = dynamic_pointer_cast<TagBigram>(other);
		if (otherDrived == NULL) return false;

		if (_t1 == otherDrived->_t1 && _t2 == otherDrived->_t2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t TagBigram::GetHash() const
	{
		string hashStr = GetMyType() + ToString(_t1) + ToString(_t2);
		return GetStrHash(hashStr);
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

	bool TagTrigram::Same(const shared_ptr<GrammarFeature> other) const
	{
		//Check type.
		auto otherDrived = dynamic_pointer_cast<TagTrigram>(other);
		if (otherDrived == NULL) return false;

		if (_t1 == otherDrived->_t1 && _t2 == otherDrived->_t2&&_t3 == otherDrived->_t3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t TagTrigram::GetHash() const
	{
		string hashStr = GetMyType() + ToString(_t1) + ToString(_t2) + ToString(_t3);
		return GetStrHash(hashStr);
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

	bool TagFollowedByWord::Same(const shared_ptr<GrammarFeature> other) const
	{
		//Check type.
		auto otherDrived = dynamic_pointer_cast<TagFollowedByWord>(other);
		if (otherDrived == NULL) return false;

		if (_word == otherDrived->_word && _t1 == otherDrived->_t1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t TagFollowedByWord::GetHash() const
	{
		string hashStr = GetMyType() + _word + ToString(_t1);
		return GetStrHash(hashStr);
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
		cmd.Bind(":word1", AsciiToUtf8(_word));
	}

	void TagFollowedByWord::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_word = row.GetText("word1");
	}

	bool WordFollowedByTag::Same(const shared_ptr<GrammarFeature> other) const
	{
		//Check type.
		auto otherDrived = dynamic_pointer_cast<WordFollowedByTag>(other);
		if (otherDrived == NULL) return false;

		if (_word == otherDrived->_word && _t1 == otherDrived->_t1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t WordFollowedByTag::GetHash() const
	{
		string hashStr = GetMyType() + _word + ToString(_t1);
		return GetStrHash(hashStr);
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
		cmd.Bind(":word1", AsciiToUtf8(_word));
	}

	void WordFollowedByTag::ReadParam(const CommonTool::DBRow& row)
	{
		_t1 = (PartOfSpeech)row.GetLong("pos1");
		_word = row.GetText("word1");
	}

}
