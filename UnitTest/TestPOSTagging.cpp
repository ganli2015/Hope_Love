#include "stdafx.h"
#include "TestPOSTagging.h"

#include "../CommonTools/CommonStringFunction.h"

#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/Word.h"
#include "../DataCollection/Sentence.h"

#include "../SentenceAnalysisAlgorithm/Punctuator.h"
#include "../SentenceAnalysisAlgorithm/WordSegmentator.h"
#include "../SentenceAnalysisAlgorithm/GrammarAnalyzer.h"


using namespace DataCollection;

TestPOSTagging::TestPOSTagging()
{
}


TestPOSTagging::~TestPOSTagging()
{
}

void TestPOSTagging::Run(const string corpusFilename, const int sentenceCount)
{
	ofstream out("Log//incorrect POS tagging.txt");

	auto samples = ReadSentences(corpusFilename, sentenceCount);
	int correctNum = 0;
	int totalNum = 0;

	for (unsigned int i = 0; i < samples.size(); ++i)
	{
		if(sentenceCount!=0 && totalNum>=sentenceCount) break;
		try
		{
			auto result = POSTagging(samples[i].raw);
			if (IsCorrect(samples[i].POSTag, result))
			{
				correctNum++;
			}
			else
			{
				OutputResult(samples[i].POSUnsplit, result, out);
			}

			++totalNum;
		}
		catch (const std::exception&)
		{
			continue;
		}

	}

	cout << "Correct Ratio: " << (double)correctNum / totalNum << endl;
	cout << "Total Number: " << totalNum << endl;
}

vector<shared_ptr<DataCollection::Word>> TestPOSTagging::POSTagging(const string sentence) const
{
	string rawSentenceString(sentence);
	Punctuator punctuator(rawSentenceString);
	shared_ptr<DataCollection::Sentence> sen(new DataCollection::Sentence(rawSentenceString));
	punctuator.Punctuate(sen);

	WordSegmentator wordsegmentor(sen);
	wordsegmentor.SetSegmentMethod(WordSegmentator::Backward);
	wordsegmentor.Segment();
	vector<shared_ptr<DataCollection::SegmentedSentence>> segmented = wordsegmentor.GetAllSegementedSentence();

	GrammarAnalyzer grammarAnalyzer(sen);
	for (unsigned int i = 0; i < segmented.size(); ++i)
	{
		grammarAnalyzer.AddSegment(segmented[i]);
	}
	grammarAnalyzer.Analyze();

	auto grammard = sen->GetGrammard();

	return grammard;
}

vector<string> TestPOSTagging::SegmentWord(const string sentence) const
{
	string rawSentenceString(sentence);
	Punctuator punctuator(rawSentenceString);
	shared_ptr<DataCollection::Sentence> sen(new DataCollection::Sentence(rawSentenceString));
	punctuator.Punctuate(sen);

	WordSegmentator wordsegmentor(sen);
	wordsegmentor.SetSegmentMethod(WordSegmentator::Backward);
	wordsegmentor.Segment();
	vector<shared_ptr<DataCollection::SegmentedSentence>> segmented = wordsegmentor.GetAllSegementedSentence();

	//Get the first segmentation.
	auto words = segmented.front()->Get();
	vector<string> res;
	for (auto word : words)
	{
		res.push_back(word->GetString());
	}

	return res;
}

vector<TestPOSTagging::POSSample> TestPOSTagging::ReadSentences(const string filename, const int sentenceCount)
{
	ifstream in(filename, ios::binary);
	if (!in)
	{
		throw runtime_error("File not found!");
	}

	vector<POSSample> res;

	int sampleNum = sentenceCount;
	int maxLength = 180;
	size_t index = 2;
	while (!in.eof())
	{
		POSSample aSample;
		aSample.raw = CommonTool::Getline_UnicodeFile(in, index);
		string POSUnsplit = CommonTool::Getline_UnicodeFile(in, index);

		//Too long or contains strange character.
		if (aSample.raw.size() > maxLength)
		{
			continue;
		}

		aSample.POSUnsplit = POSUnsplit;
		try
		{
			aSample.POSTag = FuncForTest::ParsePOSTagging(POSUnsplit);
		}
		catch (const std::exception&)
		{
			break;
		}
		res.push_back(aSample);

		if (sampleNum != 0 && res.size() >= sampleNum)
		{
			break;
		}
	}

	return res;
}



bool TestPOSTagging::IsCorrect(const vector<shared_ptr<DataCollection::Word>>& expect, const vector<shared_ptr<DataCollection::Word>>& result)
{
	if (expect.size() != result.size()) return false;

	for (unsigned int i = 0; i < expect.size(); ++i)
	{
		if (!expect[i]->IsSame(result[i]))
		{
			return false;
		}
	}

	return true;
}

void TestPOSTagging::OutputResult(const string POSUnsplit, const vector<shared_ptr<DataCollection::Word>>& results, ofstream& out)
{
	out << "Expect: " + POSUnsplit << " Result: ";
	for (unsigned int i = 0; i < results.size(); ++i)
	{
		out << results[i]->GetString() << "/" << (int)results[i]->Type() << " ";
	}
	out << endl;
}