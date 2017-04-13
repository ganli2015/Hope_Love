#include "stdafx.h"
#include "Test_GrammarModel.h"
#include "FuncForTest.h"

#include "../Mind/GrammarLocalModel.h"

#include "../DataCollection/Word.h"

using namespace DataCollection;
using namespace FuncForTest;
namespace Mind
{
	TEST(Test_GrammarLocalModel, POSwithNoLocalDistribution)
	{
		//The total count of backward or forward of a POS is zero,
		//then the result of ComputePossiblity should be not NAN.

		vector<shared_ptr<Word>> wordList;
		wordList.push_back(ToWord("沙石峪", Noun));
		wordList.push_back(ToWord("那", Pronoun));
		wordList.push_back(ToWord("瘠薄", Adjective));
		wordList.push_back(ToWord("的", Auxiliary));
		wordList.push_back(ToWord("土地", Noun));
		wordList.push_back(ToWord("和", Conjunction));
		wordList.push_back(ToWord("大寨", Noun));
		wordList.push_back(ToWord("的", Auxiliary));
		wordList.push_back(ToWord("稳产高产", Noun));
		wordList.push_back(ToWord("海绵", Noun));
		wordList.push_back(ToWord("田", Noun));
		wordList.push_back(ToWord("实在", Adverb));
		wordList.push_back(ToWord("不能", Verb));
		wordList.push_back(ToWord("相比", Verb));
		wordList.push_back(ToWord("啊", Interjection));
		wordList.push_back(ToWord("！", Noun));

		GrammarLocalModel localModel;
		localModel.ReadGrammarLocal();
		auto result = localModel.ComputePossiblity(wordList);

		ASSERT_FALSE(_isnan(result));
	}
}
