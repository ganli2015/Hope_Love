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
	class GrammarFeature;

	//Handle all feature templates.
	//<prefix> is the prefix of code expression, and <suffix> is the suffix.
#define HANDLE_ALL_FEATURETEMPLATES(prefix,suffix) prefix##TagWithWordTemplate##suffix;\
	prefix##TagBigramTemplate##suffix;\
	prefix##TagTrigramTemplate##suffix;\
	prefix##TagFollowedByWordTemplate##suffix;\
	prefix##WordFollowedByTagTemplate##suffix;\
	prefix##WordTagPreCharTemplate##suffix;\
	prefix##WordTagNextCharTemplate##suffix;\
	prefix##SingleCharWithTrigramCharTemplate##suffix;\
	prefix##WordStartWithCharTemplate##suffix;\
	prefix##WordEndWithCharTemplate##suffix;

#define HANDLE_ALL_FEATURES(prefix,suffix) prefix##TagWithWord##suffix;\
	prefix##TagBigram##suffix;\
	prefix##TagTrigram##suffix;\
	prefix##TagFollowedByWord##suffix;\
	prefix##WordFollowedByTag##suffix;\
	prefix##WordTagPreChar##suffix;\
	prefix##WordTagNextChar##suffix;\
	prefix##SingleCharWithTrigramChar##suffix;\
	prefix##WordStartWithChar##suffix;\
	prefix##WordEndWithChar##suffix;


	//Used for create a derived feature.
	//////////////////////////////////////////////////////////////////////////
	class FeatureCreator
	{
		static map<string, FeatureCreator*> _featureCreators;
		static bool _prepareTag;
	public:
		static shared_ptr<GrammarFeature> CreateFeature(const CommonTool::DBRow& row);

	protected:
		virtual shared_ptr<GrammarFeature> Create(const CommonTool::DBRow& row) = 0;
		static bool PrepareFeatureCreators();

	private:
		template<typename T>
		static void AddFeatureCreator()
		{
			auto feature = make_shared<T>();
			_featureCreators[feature->GetMyType()] = new ConcreteFeatureCreator<T>();
		}
	};

	template<class T>
	class ConcreteFeatureCreator : public FeatureCreator
	{
	protected:
		virtual shared_ptr<GrammarFeature> Create(const CommonTool::DBRow& row)
		{
			shared_ptr<T> feature(new T());
			feature->ReadFromDBRow(row);
			return feature;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	//It is a feature for some grammar.
	//It is generated from some feature template with a rule.
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT GrammarFeature : public Obj<GrammarFeature>
	{
		//////////////////////////////////////////////////////////////////////////


	protected:
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

		//////////////////////////////////////////////////////////////////////////
		//Serialize the data.
		//////////////////////////////////////////////////////////////////////////
		virtual string GetString() const = 0;
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


	};


	/************************************************************************/
	/* Features below are mainly from
	/*<<Joint Word Segmentation and POS Tagging using a Single Perceptron>>.
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	//Base for concrete feature classes with feature elements based on string or PartOfSpeech.
	//<wordSize> is the size of string array.
	//<posSize> is the size of PartOfSpeech array.
	//////////////////////////////////////////////////////////////////////////
	template<size_t wordSize, size_t posSize>
	class FeatureStyle : public GrammarFeature
	{
	private:
		string *_word;
		PartOfSpeech *_pos;

	public:
		FeatureStyle()
		{
			_word = new string[wordSize]();
			_pos = new PartOfSpeech[posSize]();
		}

		virtual ~FeatureStyle()
		{
			delete[] _word;
			delete[] _pos;
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

		virtual string GetString() const
		{
			string res = "";
			res += GetMyType();
			res += "  word: ";
			for (size_t i = 0; i < wordSize; i++)
			{
				res += _word[i]+" ";
			}
			res += "  pos: ";
			for (size_t i = 0; i < posSize; i++)
			{
				res +=CommonTool::ToString(_pos[i]) + " ";
			}

			return res;
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
				string bindParam = "word" + CommonTool::ToString(i + 1);
				_word[i] = row.GetText(bindParam);
			}
			for (size_t i = 0; i < posSize; i++)
			{
				string bindParam = "pos" + CommonTool::ToString(i + 1);
				_pos[i] = (PartOfSpeech)row.GetLong(bindParam);
			}
		}

	protected:

		string GetString(int i) const { return _word[i]; }
		void SetString(int i, const string val) { _word[i] = val; }
		PartOfSpeech GetPOS(int i) const { return _pos[i]; }
		void SetPOS(int i, const PartOfSpeech pos) { _pos[i] = pos; }
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagWithWord : public FeatureStyle<1,1>
	{
	public:
		TagWithWord() {};
		TagWithWord(const shared_ptr<Word> val)
		{
			SetString(0, val->GetString());
			SetPOS(0, val->Type());
		};
		~TagWithWord() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words) ;
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagBigram : public FeatureStyle<0, 2>
	{
	public:
		TagBigram() {};
		TagBigram(const PartOfSpeech t1, const PartOfSpeech t2) 
		{
			SetPOS(0, t1);
			SetPOS(1, t2);
		};
		~TagBigram() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag trigram t1t2t3
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagTrigram : public FeatureStyle<0, 3>
	{
	public:
		TagTrigram() {};
		TagTrigram(const PartOfSpeech t1, const PartOfSpeech t2, const PartOfSpeech t3) 
		{
			SetPOS(0, t1);
			SetPOS(1, t2);
			SetPOS(2, t3);
		};
		~TagTrigram() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t followed by word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagFollowedByWord : public FeatureStyle<1,1>
	{
	public:
		TagFollowedByWord() {};
		TagFollowedByWord(const PartOfSpeech t1, const string val) 
		{
			SetPOS(0, t1);
			SetString(0, val);
		};
		~TagFollowedByWord() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w followed by tag t
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordFollowedByTag : public FeatureStyle<1, 1>
	{
	public:
		WordFollowedByTag() {};
		WordFollowedByTag(const string val, const PartOfSpeech t1)
		{
			SetPOS(0, t1);
			SetString(0, val);
		};
		~WordFollowedByTag() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and previous character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagPreChar : public FeatureStyle<2, 1>
	{
	public:
		WordTagPreChar() {};
		WordTagPreChar(const string val, const PartOfSpeech t1, const string preC)
		{
			SetString(0, val);
			SetPOS(0, t1);
			SetString(1, preC);
		};
		~WordTagPreChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and next character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagNextChar : public FeatureStyle<2, 1>
	{
		string _word;
		PartOfSpeech _t;
		string _nextC;
	public:
		WordTagNextChar() {};
		WordTagNextChar(const string val, const PartOfSpeech t1, const string C)
		{
			SetString(0, val);
			SetPOS(0, t1);
			SetString(1, C);
		};
		~WordTagNextChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on single-character word w in character trigram c1wc2
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT SingleCharWithTrigramChar : public FeatureStyle<3, 1>
	{
	public:
		SingleCharWithTrigramChar() {};
		SingleCharWithTrigramChar(const string word, const string c1, const string c2, const PartOfSpeech t1)
		{
			SetString(0, word);
			SetString(1, c1);
			SetString(2, c2);
			SetPOS(0, t1);
		};
		~SingleCharWithTrigramChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word starting with char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordStartWithChar : public FeatureStyle<1,1>
	{
	public:
		WordStartWithChar() {};
		WordStartWithChar(const string c,  const PartOfSpeech t1) {
			SetString(0, c);
			SetPOS(0, t1);
		};
		~WordStartWithChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word ending with char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordEndWithChar : public FeatureStyle<1, 1>
	{
	public:
		WordEndWithChar() {};
		WordEndWithChar(const string c, const PartOfSpeech t1) {
			SetString(0, c);
			SetPOS(0, t1);
		};
		~WordEndWithChar() {};

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};
}


