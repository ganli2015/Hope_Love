#pragma once
#include "InOut.h"
#include <functional>
#include "Word.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/DBoperator.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
	class DBRow;
}

namespace DataCollection
{
	class Word;

	//////////////////////////////////////////////////////////////////////////
	//It is a feature for some grammar.
	//It is generated from some feature template with a rule.
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT GrammarFeature : public Obj<GrammarFeature>
	{
		//////////////////////////////////////////////////////////////////////////
		//Used for create a derived feature.
		//////////////////////////////////////////////////////////////////////////
		class FeatureCreator
		{
		public:
			virtual shared_ptr<GrammarFeature> Create(const CommonTool::DBRow& row) = 0;
		};

		template<class T>
		class ConcreteFeatureCreator : public FeatureCreator
		{
		public:
			virtual shared_ptr<GrammarFeature> Create(const CommonTool::DBRow& row)
			{
				shared_ptr<T> feature(new T());
				feature->ReadFromDBRow(row);
				return feature;
			}
		};

	protected:
		static map<string, FeatureCreator*> _featureCreators;
	public:
		GrammarFeature();
		virtual ~GrammarFeature();

		//////////////////////////////////////////////////////////////////////////
		//Get feature from a row in database.
		//////////////////////////////////////////////////////////////////////////
		static shared_ptr<GrammarFeature> GetFeature(const CommonTool::DBRow& row);

		//////////////////////////////////////////////////////////////////////////
		//Get count of this feature in this sentence.
		//////////////////////////////////////////////////////////////////////////
		virtual int FeatureCount(const vector<shared_ptr<Word>>& words);

		//////////////////////////////////////////////////////////////////////////
		//Get statement for insert row to database.
		//////////////////////////////////////////////////////////////////////////
		virtual CommonTool::DBCmd GetInsertCmd(CommonTool::DBoperator& dbOpe) const ;

		//////////////////////////////////////////////////////////////////////////
		//Read data from a row in database.
		//////////////////////////////////////////////////////////////////////////
		void ReadFromDBRow(const CommonTool::DBRow& row);

		//////////////////////////////////////////////////////////////////////////
		//Check if <other> is same with <me>.
		//////////////////////////////////////////////////////////////////////////
		virtual bool Same(const shared_ptr<GrammarFeature> other) const = 0;

		//////////////////////////////////////////////////////////////////////////
		//Get hash value.
		//////////////////////////////////////////////////////////////////////////
		virtual size_t GetHash() const = 0;

		//////////////////////////////////////////////////////////////////////////
		//Get class type of <me>.
		//////////////////////////////////////////////////////////////////////////
		string GetMyType() const;
	protected:

		

	private:
		//////////////////////////////////////////////////////////////////////////
		//Get count of feature from current word as well as its neighbour.
		//////////////////////////////////////////////////////////////////////////
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words) = 0;

		//////////////////////////////////////////////////////////////////////////
		//Bind parameter of feature to <cmd>.
		//////////////////////////////////////////////////////////////////////////
		virtual void BindParam(CommonTool::DBCmd& cmd) const = 0;

		virtual void ReadParam(const CommonTool::DBRow& row) = 0;

		static void PrepareFeatureCreators();

	};


	/************************************************************************/
	/* Features below are mainly from
	/*<<Joint Word Segmentation and POS Tagging using a Single Perceptron>>.
	/************************************************************************/

	//Check if the type of <object> is <type>,if so ,return a derived object <derived>, otherwise return false.
#define CheckType(object,type,derived) auto derived = dynamic_pointer_cast<type>(object);\
	if (derived == NULL) return false;

	//////////////////////////////////////////////////////////////////////////
	//Base for concrete feature classes with feature elements based on string or PartOfSpeech.
	//<wordSize> is the size of string array.
	//<posSize> is the size of PartOfSpeech array.
	//////////////////////////////////////////////////////////////////////////
	template<size_t wordSize, size_t posSize>
	class FeatureStyle : public GrammarFeature
	{
	protected:
		string *_word;
		PartOfSpeech *_pos;

	public:
		FeatureStyle()
		{
			_word = new string[wordSize]();
			_pos = new PartOfSpeech[posSize]();
		}

		virtual bool Same(const shared_ptr<GrammarFeature> other) const
		{
			//Check type.
			if (other->GetMyType() != GetMyType()) return false;

			auto derived = dynamic_pointer_cast<FeatureStyle<wordSize, posSize>>(other);
			assert(derived != NULL);

			//Check word.
			for (size_t i = 0; i < wordSize; i++)
			{
				if (_word[i] != derived->_word[i])
					return false;
			}

			//Check pos.
			for (size_t i = 0; i < posSize; i++)
			{
				if (_pos[i] != derived->_pos[i])
					return false;
			}

			return true;
		}

		virtual size_t GetHash() const
		{
			string hashStr = GetMyType();
			//Sum up words and pos.
			for (size_t i = 0; i < wordSize; i++)
			{
				hashStr += _word[i];
			}
			for (size_t i = 0; i < posSize; i++)
			{
				hashStr += _pos[i];
			}

			return CommonTool::GetStrHash(hashStr);
		}

	private:

		virtual void BindParam(CommonTool::DBCmd& cmd) const
		{
			//Bind word.
			for (size_t i = 0; i < wordSize; i++)
			{
				string bindParam = ":word" + CommonTool::ToString(i + 1);
				cmd.Bind(bindParam, CommonTool::AsciiToUtf8(_word[i]));
			}
			//Bind pos.
			for (size_t i = 0; i < posSize; i++)
			{
				string bindParam = ":pos" + CommonTool::ToString(i + 1);
				cmd.Bind(bindParam, (int)_pos[i]);
			}
		}

		virtual void ReadParam(const CommonTool::DBRow& row)
		{
			for (size_t i = 0; i < wordSize; i++)
			{
				string bindParam = ":word" + CommonTool::ToString(i + 1);
				_word[i] = row.GetText(bindParam);
			}
			for (size_t i = 0; i < posSize; i++)
			{
				string bindParam = ":pos" + CommonTool::ToString(i + 1);
				_pos[i] = (PartOfSpeech)row.GetLong(bindParam);
			}
		}

	};

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagWithWord : public GrammarFeature
	{
		shared_ptr<Word> _word;

	public:
		TagWithWord() {};
		TagWithWord(const shared_ptr<Word> val);
		~TagWithWord();

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words) ;
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagBigram : public GrammarFeature
	{
		PartOfSpeech _t1;
		PartOfSpeech _t2;
	public:
		TagBigram() {};
		TagBigram(const PartOfSpeech t1, const PartOfSpeech t2) :_t1(t1), _t2(t2) {};
		~TagBigram() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag trigram t1t2t3
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagTrigram : public GrammarFeature
	{
		PartOfSpeech _t1;
		PartOfSpeech _t2;
		PartOfSpeech _t3;
	public:
		TagTrigram() {};
		TagTrigram(const PartOfSpeech t1, const PartOfSpeech t2, const PartOfSpeech t3) :_t1(t1), _t2(t2), _t3(t3) {};
		~TagTrigram() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t followed by word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagFollowedByWord : public GrammarFeature
	{
		PartOfSpeech _t1;
		string _word;
	public:
		TagFollowedByWord() {};
		TagFollowedByWord(const PartOfSpeech t1, const string val) :_t1(t1), _word(val) {};
		~TagFollowedByWord() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w followed by tag t
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordFollowedByTag : public GrammarFeature
	{
		PartOfSpeech _t1;
		string _word;
	public:
		WordFollowedByTag() {};
		WordFollowedByTag(const string val, const PartOfSpeech t1) :_t1(t1), _word(val) {};
		~WordFollowedByTag() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and previous character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagPreChar : public GrammarFeature
	{
		string _word;
		PartOfSpeech _t;
		string _preC;
	public:
		WordTagPreChar() {};
		WordTagPreChar(const string val, const PartOfSpeech t1,const string preC) :_t(t1), _word(val),_preC(preC) {};
		~WordTagPreChar() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and next character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagNextChar : public GrammarFeature
	{
		string _word;
		PartOfSpeech _t;
		string _nextC;
	public:
		WordTagNextChar() {};
		WordTagNextChar(const string val, const PartOfSpeech t1, const string C) :_t(t1), _word(val), _nextC(C) {};
		~WordTagNextChar() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on single-character word w in character trigram c1wc2
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT SingleCharWithTrigramChar : public GrammarFeature
	{
		string _word;
		string _c1;
		string _c2;
		PartOfSpeech _t;
	public:
		SingleCharWithTrigramChar() {};
		SingleCharWithTrigramChar(const string word, const string c1,const string c2, const PartOfSpeech t1) :_t(t1), _word(word), _c1(c1),_c2(c2) {};
		~SingleCharWithTrigramChar() {};

		virtual bool Same(const shared_ptr<GrammarFeature> other) const;
		virtual size_t GetHash() const;

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word starting with char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordStartWithChar : public FeatureStyle<1,1>
	{
	public:
		WordStartWithChar() {};
		WordStartWithChar(const string c,  const PartOfSpeech t1) {
			_word[0] = c;
			_pos[0] = t1;
		};
		~WordStartWithChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};
}


