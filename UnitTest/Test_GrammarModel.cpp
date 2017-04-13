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
		wordList.push_back(ToWord("ɳʯ��", Noun));
		wordList.push_back(ToWord("��", Pronoun));
		wordList.push_back(ToWord("񤱡", Adjective));
		wordList.push_back(ToWord("��", Auxiliary));
		wordList.push_back(ToWord("����", Noun));
		wordList.push_back(ToWord("��", Conjunction));
		wordList.push_back(ToWord("��կ", Noun));
		wordList.push_back(ToWord("��", Auxiliary));
		wordList.push_back(ToWord("�Ȳ��߲�", Noun));
		wordList.push_back(ToWord("����", Noun));
		wordList.push_back(ToWord("��", Noun));
		wordList.push_back(ToWord("ʵ��", Adverb));
		wordList.push_back(ToWord("����", Verb));
		wordList.push_back(ToWord("���", Verb));
		wordList.push_back(ToWord("��", Interjection));
		wordList.push_back(ToWord("��", Noun));

		GrammarLocalModel localModel;
		localModel.ReadGrammarLocal();
		auto result = localModel.ComputePossiblity(wordList);

		ASSERT_FALSE(_isnan(result));
	}
}
