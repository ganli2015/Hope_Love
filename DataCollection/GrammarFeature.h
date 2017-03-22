#pragma once
#include "InOut.h"
#include "Word.h"

namespace DataCollection
{
	class Word;

	//////////////////////////////////////////////////////////////////////////
	//It is a feature for some grammar.
	//It is generated from some feature template with a rule.
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT GrammarFeature : public Obj<GrammarFeature>
	{
	public:
		GrammarFeature();
		virtual ~GrammarFeature();

		//////////////////////////////////////////////////////////////////////////
		//Get count of this feature in this sentence.
		//////////////////////////////////////////////////////////////////////////
		virtual int FeatureCount(const vector<shared_ptr<Word>>& words);

	private:
		//////////////////////////////////////////////////////////////////////////
		//Get count of feature from current word as well as its neighbour.
		//////////////////////////////////////////////////////////////////////////
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words) = 0;
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
		TagWithWord(const shared_ptr<Word> val);
		~TagWithWord();

	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words) ;
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagBigram : public GrammarFeature
	{
		PartOfSpeech _t1;
		PartOfSpeech _t2;
	public:
		TagBigram(const PartOfSpeech t1, const PartOfSpeech t2) :_t1(t1), _t2(t2) {};
		~TagBigram() {};
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
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
		TagTrigram(const PartOfSpeech t1, const PartOfSpeech t2, const PartOfSpeech t3) :_t1(t1), _t2(t2), _t3(t3) {};
		~TagTrigram() {};
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t followed by word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagFollowedByWord : public GrammarFeature
	{
		PartOfSpeech _t1;
		shared_ptr<Word> _word;
	public:
		TagFollowedByWord(const PartOfSpeech t1, const shared_ptr<Word> val) :_t1(t1),_word(val) {};
		~TagFollowedByWord() {};
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w followed by tag t
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordFollowedByTag : public GrammarFeature
	{
		PartOfSpeech _t1;
		shared_ptr<Word> _word;
	public:
		WordFollowedByTag(const shared_ptr<Word> val, const PartOfSpeech t1) :_t1(t1), _word(val) {};
		~WordFollowedByTag() {};
	private:
		virtual int CurrentFeatureCount(const unsigned i, const vector<shared_ptr<Word>>& words);
	};
}


