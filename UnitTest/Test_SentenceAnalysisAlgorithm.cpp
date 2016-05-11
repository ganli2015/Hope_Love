#include "StdAfx.h"
#include "Test_SentenceAnalysisAlgorithm.h"

#include "../SentenceAnalysisAlgorithm/Punctuator.h"
#include "../SentenceAnalysisAlgorithm/WordSegmentator.h"
#include "../SentenceAnalysisAlgorithm/GrammarAnalyzer.h"
#include "../SentenceAnalysisAlgorithm/SentenceAnalyzer.h"
#include "../SentenceAnalysisAlgorithm/WordRelationTableBuilder.h"

#include "../Mathmatic/MathTool.h"

#include "../DataCollection/Sentence.h"

#include "../MindInterface/iCerebrum.h"
#include "../Mind/Cerebrum.h"

using namespace Mind;
using namespace DataCollection;
Test_SentenceAnalysisAlgorithm::Test_SentenceAnalysisAlgorithm(void)
{
}


Test_SentenceAnalysisAlgorithm::~Test_SentenceAnalysisAlgorithm(void)
{
}

void Test_SentenceAnalysisAlgorithm::RunTest()
{
	Test_Punctuation();
	Test_Segmentation();
	Test_Grammard();
	Test_StructureAnalyzer();
	Test_WordRelationTableBuilder();
}

void Test_SentenceAnalysisAlgorithm::Test_Punctuation()
{
	string sample1_i="莫莫我爱你！";
	string sample1_o="莫莫我爱你！";
	Punctuator punct1(sample1_i);
	shared_ptr<Sentence> result1(new Sentence(sample1_i));
	punct1.Punctuate(result1);
	Check(result1->GetSubSentence(0)==sample1_o);

	string sample2_i="干干，来接我了吗";
	string sample2_o1="干干，";
	string sample2_o2="来接我了吗";
	Punctuator punct2(sample2_i);
	shared_ptr<Sentence> result2(new Sentence(sample2_i));
	punct2.Punctuate(result2);
	Check(result2->GetSubSentence(0)==sample2_o1 && result2->GetSubSentence(1)==sample2_o2);

	string sample3_i="她说：“我们去玩吧！”";
	string sample3_o1="她说：";
	string sample3_o2="“我们去玩吧！”";
	Punctuator punct3(sample3_i);
	shared_ptr<Sentence> result3(new Sentence(sample3_i));
	punct3.Punctuate(result3);
	Check(result3->GetSubSentence(0)==sample3_o1 && result3->GetSubSentence(1)==sample3_o2);
}

void Test_SentenceAnalysisAlgorithm::Test_Segmentation()
{
	iCerebrum* brain=iCerebrum::Instance();

	{
		string sample_i="莫莫我爱你！";
		vector<string> expect;
		expect.push_back("莫莫");
		expect.push_back("我");
		expect.push_back("爱");
		expect.push_back("你");
		expect.push_back("！");
		shared_ptr<Sentence> sen(new Sentence(sample_i));
		sen->AddSubSentence(sample_i);
		WordSegmentator wordSegmentator(sen);
		wordSegmentator.Segment();
		vector<shared_ptr<SegmentedSentence>> res=wordSegmentator.GetAllSegementedSentence();
		Check(res.size()==1 && SameSentence(expect,res.front()));
	}

	{
		string sample_i="今天去哪里？";
		vector<string> expect1;
		expect1.push_back("今天");
		expect1.push_back("去");
		expect1.push_back("哪");
		expect1.push_back("里");
		expect1.push_back("？");
		vector<string> expect2;
		expect2.push_back("今天");
		expect2.push_back("去");
		expect2.push_back("哪里");
		expect2.push_back("？");

		shared_ptr<Sentence> sen(new Sentence(sample_i));
		sen->AddSubSentence(sample_i);
		WordSegmentator wordSegmentator(sen);
		wordSegmentator.Segment();
		vector<shared_ptr<SegmentedSentence>> res=wordSegmentator.GetAllSegementedSentence();
		Check(res.size()==2 && SameSentence(expect1,res.front()));
		Check(res.size()==2 && SameSentence(expect2,res[1]));
	}

	{
		string sample_i="动物园";
		vector<string> expect1;
		expect1.push_back("动");
		expect1.push_back("物");
		expect1.push_back("园");
		vector<string> expect2;
		expect2.push_back("动物");
		expect2.push_back("园");
		vector<string> expect3;
		expect3.push_back("动");
		expect3.push_back("物园");
		vector<string> expect4;
		expect4.push_back("动物园");

		shared_ptr<Sentence> sen(new Sentence(sample_i));
		sen->AddSubSentence(sample_i);
		WordSegmentator wordSegmentator(sen);
		wordSegmentator.Segment();
		vector<shared_ptr<SegmentedSentence>> res=wordSegmentator.GetAllSegementedSentence();
		Check(res.size()==4 && SameSentence(expect1,res.front()));
		Check(res.size()==4 && SameSentence(expect3,res[1]));
		Check(res.size()==4 && SameSentence(expect2,res[2]));
		Check(res.size()==4 && SameSentence(expect4,res[3]));
	}
}

bool Test_SentenceAnalysisAlgorithm::SameSentence( const vector<string> expect,const shared_ptr<SegmentedSentence> res )
{
	vector<shared_ptr<Word>> words=res->Get();

	if(words.size()!=expect.size())
	{
		return false;
	}

	for (unsigned int i=0;i<words.size();++i)
	{
		if(words[i]->GetString()!=expect[i])
		{
			return false;
		}
	}

	return true;
}

void Test_SentenceAnalysisAlgorithm::Test_Grammard()
{
	{
		string sample_i="莫莫我爱你！";
		shared_ptr<Sentence> sen(new Sentence(sample_i));
		sen->AddSubSentence(sample_i);

		vector<shared_ptr<Word>> segWords;
		segWords.push_back(shared_ptr<Word>(new Word("莫莫")));
		segWords.push_back(shared_ptr<Word>(new Word("我")));
		segWords.push_back(shared_ptr<Word>(new Word("爱")));
		segWords.push_back(shared_ptr<Word>(new Word("你")));
		segWords.push_back(shared_ptr<Word>(new puncture("！")));

		GrammarAnalyzer grammarAnalyzer(sen);
		grammarAnalyzer.AddSegment(shared_ptr<SegmentedSentence>(new SegmentedSentence(segWords)));
		grammarAnalyzer.Analyze();

		vector<PartOfSpeech> expect;
		expect.push_back(Pronoun);
		expect.push_back(Pronoun);
		expect.push_back(Verb);
		expect.push_back(Pronoun);
		expect.push_back(Puncture);
		vector<shared_ptr<Word>> graWords=sen->GetGrammard();
		Check(SameGrammar(expect,graWords));
	}
}

bool Test_SentenceAnalysisAlgorithm::SameGrammar( const vector<PartOfSpeech>& expect,const vector<shared_ptr<Word>>& words )
{
	if(expect.size()!=words.size())
	{
		return false;
	}

	for (unsigned int i=0;i<expect.size();++i)
	{
		if(expect[i]!=words[i]->Type())
		{
			return false;
		}
	}

	return true;
}

void Test_SentenceAnalysisAlgorithm::Test_WordRelationTableBuilder()
{
	//Check that the grammar interaction only affects the adjacent words.
	FuncForTest::AddGrammarPatternToCerebrum();

	SentenceAnalyzer analyzer("二大于一");
	analyzer.Analyze();
	shared_ptr<Sentence> sen=analyzer.GetAnalyzedSentences();

	WordRelationTableBuilder builder(sen);
	builder.Build();
	shared_ptr<iConceptInteractTable> table=builder.GetProtoInteractTable();

	vector<pair<string,string>> expect;
	expect.push_back(make_pair("二","大"));
	expect.push_back(make_pair("大","于"));
	expect.push_back(make_pair("于","一"));

	Check(FuncForTest::PairSameWithTable(expect,table));

	iCerebrum::KillInstance();
	iCerebrum::SetInstance(Cerebrum::Instance());
}

void Test_SentenceAnalysisAlgorithm::Test_StructureAnalyzer()
{
	//Check that the grammar interaction only affects the adjacent words.
	FuncForTest::AddGrammarPatternToCerebrum();

	SentenceAnalyzer analyzer("二大于一");
	analyzer.Analyze();
	shared_ptr<Sentence> sen=analyzer.GetAnalyzedSentences();
	for (unsigned int i=0;i<sen->GrammarWordCount();++i)
	{
		for (unsigned int j=0;j<sen->GrammarWordCount();++j)
		{
			double inten=sen->GetWordIntensity(i,j);
			if(j-i==1 || i-j==1)
			{
				Check(inten!=0);
			}
			else
			{
				Check(Math::DoubleCompare(inten,0)==0);
			}
		}
	}

	iCerebrum::KillInstance();
	iCerebrum::SetInstance(Cerebrum::Instance());
}
