#pragma once
#include "InOut.h"

namespace DataCollection
{
	class GrammarFeature;
	class Word;
	class Character;

	//////////////////////////////////////////////////////////////////////////
	//It represents an abstract feature for grammar.
	//It is a rule to tell what is a feature.
	//After it interacts with a sentence, it generates several concrete features from the sentence.
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT GrammarFeatureTemplate : public Obj<GrammarFeature>
	{
	public:
		GrammarFeatureTemplate();
		virtual ~GrammarFeatureTemplate();

		virtual vector<shared_ptr<GrammarFeature>> FindFeatures(const vector<shared_ptr<Word>>& words) ;

	private:
		//////////////////////////////////////////////////////////////////////////
		//Find a feature from current word as well as its neighbour.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words) = 0;
	
	protected:
		string PickRandomMiddleChar(const vector<Character>& characters) const;
	
	};


	/************************************************************************/
	/* Feature templates below are mainly from
	/*<<Joint Word Segmentation and POS Tagging using a Single Perceptron>>.
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	//tag t with word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagWithWordTemplate : public GrammarFeatureTemplate
	{
	public:
		TagWithWordTemplate();
		virtual ~TagWithWordTemplate();

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words) ;
	};

	//////////////////////////////////////////////////////////////////////////
	//tag bigram t1t2
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagBigramTemplate : public GrammarFeatureTemplate
	{
	public:
		TagBigramTemplate() {};
		virtual ~TagBigramTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag trigram t1t2t3
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagTrigramTemplate : public GrammarFeatureTemplate
	{
	public:
		TagTrigramTemplate() {};
		virtual ~TagTrigramTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t followed by word w
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT TagFollowedByWordTemplate : public GrammarFeatureTemplate
	{
	public:
		TagFollowedByWordTemplate() {};
		virtual ~TagFollowedByWordTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w followed by tag t
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordFollowedByTagTemplate : public GrammarFeatureTemplate
	{
	public:
		WordFollowedByTagTemplate() {};
		virtual ~WordFollowedByTagTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and previous character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagPreCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordTagPreCharTemplate() {};
		virtual ~WordTagPreCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//word w with tag t and next character c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordTagNextCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordTagNextCharTemplate() {};
		virtual ~WordTagNextCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};


	//////////////////////////////////////////////////////////////////////////
	//tag t on single-character word w in character trigram c1wc2
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT SingleCharWithTrigramCharTemplate : public GrammarFeatureTemplate
	{
	public:
		SingleCharWithTrigramCharTemplate() {};
		virtual ~SingleCharWithTrigramCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word starting with char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordStartWithCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordStartWithCharTemplate() {};
		virtual ~WordStartWithCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};


	//////////////////////////////////////////////////////////////////////////
	//tag t on a word ending with char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordEndWithCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordEndWithCharTemplate() {};
		virtual ~WordEndWithCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word containing char c (not the starting or ending character)
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordContainCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordContainCharTemplate() {};
		virtual ~WordContainCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word starting with char c0 and containing char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordContainCharStartWithCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordContainCharStartWithCharTemplate() {};
		virtual ~WordContainCharStartWithCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word ending with char c0 and containing char c
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordContainCharEndWithCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordContainCharEndWithCharTemplate() {};
		virtual ~WordContainCharEndWithCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};

	//////////////////////////////////////////////////////////////////////////
	//tag t on a word containing repeated char cc
	//////////////////////////////////////////////////////////////////////////
	class _DATACOLLECTIONINOUT WordWithRepeatedCharTemplate : public GrammarFeatureTemplate
	{
	public:
		WordWithRepeatedCharTemplate() {};
		virtual ~WordWithRepeatedCharTemplate() {};

	private:
		virtual shared_ptr<GrammarFeature> FindOneFeature(const unsigned i, const vector<shared_ptr<Word>>& words);
	};
}


