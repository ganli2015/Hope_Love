#pragma once
#include "InOut.h"
#include <functional>
#include "Word.h"

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

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
		virtual void BindParam(CommonTool::DBCmd& cmd) const;
		virtual void ReadParam(const CommonTool::DBRow& row);
	};
}


