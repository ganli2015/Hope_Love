#include "StdAfx.h"
#include "WordSegmentator.h"

#include "../DataCollection/Sentence.h"
#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

#include "../CommonTools/LogWriter.h"

#include "../MindInterface/iCerebrum.h"

#include "../Mathmatic/FindSequence.h"

using namespace std;
using namespace DataCollection;



WordSegmentator::WordSegmentator(shared_ptr<DataCollection::Sentence> sentence):_unsegmented(sentence),_segMethod(Forward)
{
}


WordSegmentator::~WordSegmentator(void)
{
}

void WordSegmentator::GetAllPossibleSequentialCombine(const vector<shared_ptr<Word>>& words, vector<vector<shared_ptr<Word>>>& combinations)
{
	//Get all continuous combinations of a word sequence.
	//For example, a b c, the result is a b c, ab c, a bc, and abc.
	combinations.clear();
	if(words.empty()) return;
	if(words.size()==1) 
	{
		combinations.push_back(words);
		return ;
	}

	//Use divide-and-conquer to get all combinations.
	//Each combination can be divide into two parts:
	//first few continuous words and all combinations of remaining words.
	//Then connect them.
	for (size_t i=0;i<words.size();++i)
	{
		//Collect first few continuous words.
		Word newword=*words[0];
		for (size_t j=1;j<=i;++j)
		{
			newword=newword+*words[j];
		}

		//Get all combinations of remaining words.
		vector<shared_ptr<Word>> restWords;
		restWords.insert(restWords.end(),words.begin()+i+1,words.end());
		vector<vector<shared_ptr<Word>>> tmp_combinations;
		GetAllPossibleSequentialCombine(restWords,tmp_combinations);
		
		//Connect.
		vector<vector<shared_ptr<Word>>> newtmp_combinations = ConnectCurrentWordWithSubCombinations(newword, tmp_combinations);	

		combinations.insert(combinations.end(),newtmp_combinations.begin(),newtmp_combinations.end());
	}
}

vector<vector<shared_ptr<Word>>> WordSegmentator::ConnectCurrentWordWithSubCombinations(const Word& newword,const vector<vector<shared_ptr<Word>>>& sub_combinations)
{
	//Each sub combination contributes to new combinations.
	vector<vector<shared_ptr<Word>>> newtmp_combinations;
	if (sub_combinations.empty())
	{
		vector<shared_ptr<Word>> new_combination;
		new_combination.push_back(shared_ptr<Word>(new Word(newword)));
		newtmp_combinations.push_back(new_combination);
	}
	else
	{
		for (size_t j = 0; j < sub_combinations.size(); ++j)
		{
			vector<shared_ptr<Word>> new_combination;
			new_combination.push_back(shared_ptr<Word>(new Word(newword)));
			new_combination.insert(new_combination.end(), sub_combinations[j].begin(), sub_combinations[j].end());
			newtmp_combinations.push_back(new_combination);
		}
	}

	return newtmp_combinations;
}

void WordSegmentator::AppendLastKnownWordsToCombinations(const vector<shared_ptr<Word>>& words, const vector<vector<int>>& seqs_UandA, vector<vector<shared_ptr<Word>>>& combinations)
{
	//Collect the words behind the last U_A word.
	//As it reaches the end of U_A word, remaining words must be known words and 
	//there is only one combination.
	vector<shared_ptr<Word>> lastWords;
	int startIndex = *seqs_UandA.back().rbegin() + 1;
	for (size_t i = startIndex; i < words.size(); ++i)
	{
		lastWords.push_back(words[i]);
	}

	//Connect combinations with the last words.
	for (size_t i = 0; i < combinations.size(); ++i)
	{
		vector<shared_ptr<Word>> newComb = combinations[i];
		newComb.insert(newComb.end(), lastWords.begin(), lastWords.end());
		combinations[i] = newComb;
	}
}

vector<shared_ptr<Word>> WordSegmentator::CollectKnownWordsBetweenUandAWords(const vector<shared_ptr<Word>>& words, const vector<vector<int>>& seqs_UandA, const int index)
{
	//Collect the words between the end U_A word of last sequence and the start U_A word of current  sequence.
	//If <index> equals to 0, <forwardWords> is empty. 
	vector<shared_ptr<Word>> forwardWords;
	int seqStartIndex = seqs_UandA[index][0];
	int index_prev;
	if (index == 0)
	{
		index_prev = 0;
	}
	else
	{
		index_prev = *seqs_UandA[index - 1].rbegin() + 1;
	}
	for (int i = index_prev; i < seqStartIndex; ++i)
	{
		forwardWords.push_back(words[i]);
	}

	return forwardWords;
}

vector<vector<shared_ptr<Word>>> WordSegmentator::GenerateNewCombinations(const vector<shared_ptr<Word>>& forwardWords,
	const vector<vector<shared_ptr<Word>>>& combinations_UandA, vector<vector<shared_ptr<Word>>>& combinations)
{
	vector<vector<shared_ptr<Word>>> newCombinations;
	for (size_t i = 0; i < combinations_UandA.size(); ++i)
	{
		//If <index> is zero, <combinations> is empty.
		//Then connect  <forwardWords> and current U_A words for the first combination.
		//Otherwise, there are several combinations between <forwardWords> and 
		//I should connect <forwardWords> with each of combinations before it as well as current U_A words.
		if (combinations.empty())
		{
			vector<shared_ptr<Word>> aCombination;
			aCombination.insert(aCombination.end(), forwardWords.begin(), forwardWords.end());
			aCombination.insert(aCombination.end(), combinations_UandA[i].begin(), combinations_UandA[i].end());
			newCombinations.push_back(aCombination);
		}
		else
		{
			for (size_t j = 0; j < combinations.size(); ++j)
			{
				vector<shared_ptr<Word>> aCombination;
				aCombination.insert(aCombination.end(), combinations[j].begin(), combinations[j].end());
				aCombination.insert(aCombination.end(), forwardWords.begin(), forwardWords.end());
				aCombination.insert(aCombination.end(), combinations_UandA[i].begin(), combinations_UandA[i].end());
				newCombinations.push_back(aCombination);
			}
		}
	}

	return newCombinations;
}

void WordSegmentator::MergeCombination_UandA(const vector<shared_ptr<Word>>& words, const vector<vector<int>>& seqs_UandA , const int index,vector<vector<shared_ptr<Word>>>& combinations)
{
	//After reach the end of <seqs_UandA>, append the words behind the last U_A word to combinations.
	if(index==seqs_UandA.size())
	{
		AppendLastKnownWordsToCombinations(words, seqs_UandA, combinations);
		return ;
	}

	vector<shared_ptr<Word>> forwardWords = CollectKnownWordsBetweenUandAWords(words, seqs_UandA, index);
	
	//Collect all U_A words of current sequence and search all possible combinations.
	vector<shared_ptr<Word>> uandAwords(seqs_UandA[index].size());
	for (size_t i=0;i<seqs_UandA[index].size();++i)
	{
		uandAwords[i]=words[seqs_UandA[index][i]];
	}
	vector<vector<shared_ptr<Word>>> combinations_UandA;
	GetAllPossibleSequentialCombine(uandAwords,combinations_UandA);

	//Go through every U_A word combination.
	//And its diverse combination contribute to final diversity of sentence combinations.
	vector<vector<shared_ptr<Word>>> newCombinations=GenerateNewCombinations(forwardWords, combinations_UandA, combinations);

	//Step into the next U_A word sequence and continue to explore combinations.
	combinations=newCombinations;
	MergeCombination_UandA(words,seqs_UandA,index+1,combinations);

}

void WordSegmentator::SegmentMannersAccordingToUandA(const vector<shared_ptr<Word>>& words, vector<vector<shared_ptr<Word>>>& segmented)
{
	//Collect all continuous U_A words in <words> and each of sequences will append to <seqs_UandA>.
	//There are many combinations in each continuous U_A sequence.
	vector<vector<int>> seqs_UandA=ComputeUandAIndexes(words); 

	//If the number of continuous U&A words is too larger, then do not find all combination of it yet.
	size_t largestUandAWords = 5;
	if (seqs_UandA.size() > largestUandAWords)
	{
		segmented.push_back(words);
		return;
	}
	for (vector<vector<int>>::iterator it=seqs_UandA.begin();it!=seqs_UandA.end();)
	{
		if (it->size() > largestUandAWords)
		{
			it = seqs_UandA.erase(it);
		}
		else
		{
			++it;
		}
	}

	//If there is no U_A words, then there is only one combination.
	//Otherwise, go through all U_A words and find all combinations.
	if(seqs_UandA.empty())
	{
		segmented.push_back(words);
		return ;
	}
	else
	{
		MergeCombination_UandA(words,seqs_UandA,0,segmented);
	}

}

vector<vector<int>> WordSegmentator::ComputeUandAIndexes(const vector<shared_ptr<Word>>& words)
{
	Mind::iCerebrum *brain = Mind::iCerebrum::Instance();

	size_t i = 0;
	vector<vector<int>> seqs_UandA;
	while (i < words.size())
	{
		vector<int> seq_UandA;
		//Search for continuous U_A words.
		while (i < words.size() && brain->GetAllKindsofWord(words[i]).empty())//if the word is unknown or ambiguous
		{
			seq_UandA.push_back(i++);
		}

		if (!seq_UandA.empty())
		{
			seqs_UandA.push_back(seq_UandA);
		}
		++i;
	}

	return seqs_UandA;
}

bool WordSegmentator::Segment(  )
{
	NDC_SECTION("Word Segmentation");
	for (size_t i=0;i<_unsegmented->Count_SubSentence();++i)
	{
		SegmentSubsentence(_unsegmented->GetSubSentence(i));
	}

 	return true;
}

void WordSegmentator::SegmentSubsentence( const string subsentence )
{
	//Separate words with punctuations as only words need to be segmented.
	vector<Character> raw = ConvertStringToCharacter(subsentence);
	pair<vector<Character>, vector<Character>> sen_punc = LanguageFunc::TrimEndPunctures(raw);
	vector<Character> raw_noPunc = sen_punc.first;
	vector<Character> punc = sen_punc.second;

	vector<shared_ptr<Word>> initial_segmented ;
	if (_segMethod == Forward)
	{
		initial_segmented = ForwardSegment(raw_noPunc);
	}
	else if (_segMethod == Backward)
	{
		initial_segmented = BackwardSegment(raw_noPunc);
	}

	//Collect all combinations according to U_A words.
	//How to determine the desired combination is things behind.
	vector<vector<shared_ptr<Word>>> segmented;
	SegmentMannersAccordingToUandA(initial_segmented,segmented);

	//Recover punctuations.
	vector<shared_ptr<Word>> punc_words=LanguageFunc::ConvertPuncturesToWords(punc);
	for (size_t i=0;i<segmented.size();++i)
	{
		vector<shared_ptr<Word>> seg_withPunc=segmented[i];
		seg_withPunc.insert(seg_withPunc.end(),punc_words.begin(),punc_words.end());
		
		_segmented.insert(make_pair(subsentence,shared_ptr<SegmentedSentence>(new SegmentedSentence(seg_withPunc))));
	}
}

vector<shared_ptr<Word>> WordSegmentator::ForwardSegment(const vector<Character>& raw_noPunc) const
{
	vector<CharacterProperty> vec_characterProperty;
	//Find the candidate word of each character.
	//The first character of each word is <chara>.
	for (size_t i = 0; i < raw_noPunc.size(); ++i)
	{
		Character chara = raw_noPunc[i];
		CharacterProperty characterProperty = GenerateCharacterProperty(chara, i, raw_noPunc);

		vec_characterProperty.push_back(characterProperty);
	}

	//Pick the longest candidate word of each character to compose the sentence.
	//It is experiential.We assume the sentence made with fewest words as possible.
	size_t index(0);
	vector<shared_ptr<Word>> initial_segmented;
	while (index < raw_noPunc.size())
	{
		Word candidate = *(vec_characterProperty[index].candidate.rbegin());
		int step = candidate.NumOfChara();
		initial_segmented.push_back(shared_ptr<Word>(new Word(candidate)));
		index += step;
	}

	return initial_segmented;
}

vector<shared_ptr<DataCollection::Word>> WordSegmentator::BackwardSegment(const vector<DataCollection::Character>& raw_noPunc) const
{
	Mind::iCerebrum* brain = Mind::iCerebrum::Instance();

	vector<shared_ptr<DataCollection::Word>> res;
	if (raw_noPunc.empty()) return res;

	//Max length of a word.
	int maxLength = 4;
	//Find words from end of sentence.
	for (int i=(int)raw_noPunc.size()-1;i>=0;)
	{
		Word curWord;
		shared_ptr<Word> longestWord=NULL;
		//Check four words end with current character.
		for (int j = i; j >=i-maxLength+1 && j >= 0; --j)
		{
			curWord = raw_noPunc[j]+curWord;
			if (brain->IsInMind(curWord.GetString()))
			{
				longestWord = shared_ptr<Word>(new Word(curWord));
			}
		}

		if (longestWord == NULL)
		{
			//Current character is an unknown word.
			res.push_back(shared_ptr<Word>(new Word(raw_noPunc[i].GetString())));
			--i;
		}
		else
		{
			//Forward index i
			i -= longestWord->NumOfChara();
			res.push_back(longestWord);
		}
	}

	reverse(res.begin(), res.end());

	return res;
}

WordSegmentator::CharacterProperty WordSegmentator::GenerateCharacterProperty(const Character& chara, const int myIndex, const vector<Character>& raw_noPunc) const
{
	Mind::iCerebrum* brain=Mind::iCerebrum::Instance();

	CharacterProperty characterProperty;
	characterProperty.character=chara;
	characterProperty.index=myIndex;
	Word curWord(chara.GetString());
	if(brain->IsInMind(shared_ptr<Word>(new Word(curWord))))
	{
		curWord.KnowIt();
	}
	//The current character must be a candidate whether it is unknown or not.
	//This is for the following computation.
	characterProperty.candidate.push_back(curWord);

	//Determine the longest candidate of <character>.
	//We do not need to search to the end of the sentence, but only to search to the max possible length of candidates.
	size_t maxLength_Word=brain->MaxLength_WordWithHead(shared_ptr<Character>(new Character(chara)));//Get the max length of the forward adjacent word, to determine how further I should search in the raw sentence.
	Word possibleWord(chara.GetString());//find the possible word related with the character
	for (size_t j=1;j<=maxLength_Word;++j)
	{
		if(myIndex+j>=raw_noPunc.size())//if exceed the raw sentence.
			break;

		possibleWord+=raw_noPunc[myIndex+j];
		if(brain->IsInMind(shared_ptr<Word> (new Word(possibleWord))))
		{
			possibleWord.KnowIt();
			characterProperty.candidate.push_back(possibleWord);
		}
	}

	return characterProperty;
}

std::vector<Character> WordSegmentator::GetRawSentence( shared_ptr<DataCollection::Sentence> sentence ) const
{
	vector<shared_ptr<Character>> raw=sentence->GetRawSentence();
	vector<Character> res;
	for (size_t i=0;i<raw.size();++i)
	{
		res.push_back(*raw[i]);
	}

	return res;
}

std::vector<DataCollection::Character> WordSegmentator::ConvertStringToCharacter( const string str ) const
{
	vector<shared_ptr<Character>> ptrCharas=LanguageFunc::ConvertStringToCharacter(str);
	vector<Character> res;
	res.reserve(ptrCharas.size());
	for (size_t i=0;i<ptrCharas.size();++i)
	{
		res.push_back(*ptrCharas[i]);
	}

	return res;
}

vector<shared_ptr<DataCollection::SegmentedSentence>> WordSegmentator::GetAllSegementedSentence() const
{
	//Collect all manners of segmentation of all sub sentences.
	//Each element of <subSentenSeg> is all manners of segmentation of one sub sentence.
	vector<vector<shared_ptr<SegmentedSentence>>> subSentenSeg; 
	for (size_t i=0;i<_unsegmented->Count_SubSentence();++i)
	{
		string subStr=_unsegmented->GetSubSentence(i);
		multimap<string,shared_ptr<SegmentedSentence>>::const_iterator beg=_segmented.lower_bound(subStr);
		multimap<string,shared_ptr<SegmentedSentence>>::const_iterator end=_segmented.upper_bound(subStr);

		vector<shared_ptr<SegmentedSentence>> allSeg;
		while(beg!=end)
		{
			allSeg.push_back(beg->second);
			beg++;
		}

		subSentenSeg.push_back(allSeg);
	}

	//Go through all combinations of each sub sentence and connect them into all possible sentence.
	vector<vector<shared_ptr<SegmentedSentence>>> segSequence= Math::GetAllCombinations<shared_ptr<SegmentedSentence>>::Get(subSentenSeg);
	
	vector<shared_ptr<SegmentedSentence>> res;
	res.reserve(segSequence.size());
	for (size_t i=0;i<segSequence.size();++i)
	{
		vector<shared_ptr<Word>> wholeWords;
		vector<shared_ptr<SegmentedSentence>> oneSequence=segSequence[i];
		for (size_t j=0;j<oneSequence.size();++j)
		{
			vector<shared_ptr<Word>> oneSeqWords=oneSequence[j]->Get();
			wholeWords.insert(wholeWords.end(),oneSeqWords.begin(),oneSeqWords.end());
		}

		res.push_back(shared_ptr<SegmentedSentence>(new SegmentedSentence(wholeWords)));
	}

	return res;
}


