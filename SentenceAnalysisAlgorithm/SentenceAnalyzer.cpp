#include "StdAfx.h"
#include "SentenceAnalyzer.h"
#include "Punctuator.h"
#include "WordSegmentator.h"
#include "GrammarAnalyzer.h"
#include "StructureAnalyzer.h"

#include "../CommonTools/CommonTranslateFunction.h"
#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/Character.h"
#include "../DataCollection/Sentence.h"
#include "../DataCollection/Word.h"

#include "../MindInterface/iCerebrum.h"
#include "../MindElement/Concept.h"

#include "../CommonTools/LogWriter.h"
#include "../CommonTools/CommonStringFunction.h"

#include <iostream>

using namespace std;
using namespace DataCollection;
 

SentenceAnalyzer::SentenceAnalyzer(void)
{
}

SentenceAnalyzer::SentenceAnalyzer(std::string sen):_rawSentenceString(sen)
{
	
}

SentenceAnalyzer::~SentenceAnalyzer(void)
{
	//DeleteVectorPtr(_punctuated);
}

void SentenceAnalyzer::Analyze()
{
	LOG("SentenceAnalyzer: Begin");

	Mind::iCerebrum* brain=Mind::iCerebrum::Instance();
	//Punctuate the sentence into several sub sentence.
	Punctuator punctuator(_rawSentenceString);
	shared_ptr<DataCollection::Sentence> sentence(new Sentence(_rawSentenceString));

	LOG("Begin punctuate sentence.");
	punctuator.Punctuate(sentence);
	LOG("Finish punctuate sentence.");

	//Segment the sentence and get all manners of segmented sentences.
	WordSegmentator wordsegmentor(sentence);
	LOG("Begin segment sentence.");
	wordsegmentor.Segment();
	vector<shared_ptr<SegmentedSentence>> segmented=wordsegmentor.GetAllSegementedSentence();
	LOG("Finish segment sentence.");

	//Compute the optimal POS of each words in the sentence.
	GrammarAnalyzer grammarAnalyzer(sentence);
	LOG("Begin analyze grammar of sentence.");
	for (unsigned int i = 0; i < segmented.size(); ++i)
	{
		grammarAnalyzer.AddSegment(segmented[i]);
	}
	grammarAnalyzer.Analyze();
	LOG("Finish analyze grammar of sentence.");

	DEBUG_DESC("The raw sentence is ", LanguageFunc::ConvertCharacterToString(sentence->GetRawSentence()));
	Cout_GrammardSentence(sentence);

	//Compute the relationship between words in the sentence.
	StructureAnalyzer structureAnalyzer(sentence);
	LOG("Begin analyze structure of sentence.");
	structureAnalyzer.Analyze();
	LOG("Finish analyze structure of sentence.");

	DEBUG_DESC("The raw sentence is ", LanguageFunc::ConvertCharacterToString(sentence->GetRawSentence()));
	Cout_WordConnectionIntensity(sentence);

	//Count unknown words.
	_unknownWords=CountUnknownWords(sentence);

	_analyzedSentences=sentence;
}

void SentenceAnalyzer::Cout_GrammardSentence(const shared_ptr<Sentence> grammard)
{
	vector<shared_ptr<Word>> instance=grammard->GetGrammard();
	DEBUGLOG("Grammar :");

	for(unsigned int j=0;j<instance.size();++j)
	{
		shared_ptr<Word> word=instance[j];
		DEBUGLOG(word->GetString() + " :" + CommonTool::ToString(word->Type()));
	}
}

void SentenceAnalyzer::Cout_WordConnectionIntensity( const shared_ptr<DataCollection::Sentence> sentence )
{
	int wordCount = sentence->GrammarWordCount();
	for (int j = 0; j < wordCount; ++j)
	{
		for (int k = 0; k < wordCount; ++k)
		{
			//DEBUGLOG(sentence->GetWordIntensity(j, k));
		}
	}
}

vector<shared_ptr<DataCollection::Word>> SentenceAnalyzer::CountUnknownWords( const shared_ptr<Sentence>& sentences ) const
{
	Mind::iCerebrum* brain=Mind::iCerebrum::Instance();

	vector<shared_ptr<DataCollection::Word>> res;

	vector<shared_ptr<Word>> words=sentences->GetGrammard();
	for (unsigned int j=0;j<words.size();++j)
	{
		if(words[j]->Type()==Punctuation)
		{
			continue;
		}
		if(!brain->IsInMind(words[j]))
		{
			res.push_back(words[j]);
		}
	}

	return res;
}
