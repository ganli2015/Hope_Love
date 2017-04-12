#pragma once
#include "PublicHeader.h"

namespace DataCollection {
	enum PartOfSpeech;
	class Word;
}

class TestPOSTagging
{
	struct POSSample
	{
		string raw;
		vector<shared_ptr<DataCollection::Word>> POSTag;
		string POSUnsplit;
	};

public:
	TestPOSTagging();
	~TestPOSTagging();

	//////////////////////////////////////////////////////////////////////////
	//Test sentences in the file <corpusFilename>. The number of sentence set to <sentenceCount>.
	//////////////////////////////////////////////////////////////////////////
	void Run(const string corpusFilename,const int sentenceCount);

private:
	vector<shared_ptr<DataCollection::Word>> POSTagging(const string sentence) const;

	vector<TestPOSTagging::POSSample> ReadSentences(const string filename, const int sentenceCount);

	vector<shared_ptr<DataCollection::Word>> ParsePOSTagging(const string line);

	bool IsCorrect(const vector<shared_ptr<DataCollection::Word>>& expect, const vector<shared_ptr<DataCollection::Word>>& result);

	void OutputResult(const string POSUnsplit, const vector<shared_ptr<DataCollection::Word>>& result, ofstream& out);
};
