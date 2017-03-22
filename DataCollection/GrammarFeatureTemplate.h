#pragma once
#include "InOut.h"

namespace DataCollection
{
	class GrammarFeature;
	class Word;

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
}


