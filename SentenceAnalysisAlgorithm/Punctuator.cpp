#include "StdAfx.h"
#include "Punctuator.h"
#include "../DataCollection/Character.h"
#include "../DataCollection/Sentence.h"
#include "../DataCollection/DataBaseProcessorTool.h"

using namespace DataCollection;
using namespace std;

Punctuator::Punctuator(std::string longSen):_unpunctuated(new Sentence(longSen))
{
}


Punctuator::~Punctuator(void)
{
}

bool Punctuator::Punctuate( std::vector<shared_ptr<DataCollection::Sentence>>& punctuated )
{
	punctuated.clear();
	vector<shared_ptr<Character>> unpun=_unpunctuated->GetRawSentence();

	if(unpun.empty())
		return false;
	vector<shared_ptr<Character>>::iterator sen_it=unpun.begin();
	do 
	{
		vector<shared_ptr<Character>>::iterator chara_it=find_if(sen_it,unpun.end(),DataBaseProcessorTool::IsPuncEndofSentence);

		if(chara_it==unpun.end()) //if there is no punctuation in the end
		{
			vector<shared_ptr<Character>> aSen(sen_it,chara_it);
			shared_ptr<Sentence> pun(new Sentence(aSen));
			punctuated.push_back(pun);
			break;
		}

		//find the next character which is not a punctuation
		do 
		{
			++chara_it;
			if(chara_it==unpun.end())
			{
				vector<shared_ptr<Character>> aSen(sen_it,chara_it);
				shared_ptr<Sentence> pun(new Sentence(aSen));
				punctuated.push_back(pun);
				break;
			}
			else if(DataBaseProcessorTool::IsPuncEndofSentence(*chara_it)||DataBaseProcessorTool::IsPuncRightside(*chara_it))
				continue;
			else 
			{
				vector<shared_ptr<Character>> aSen(sen_it,chara_it);
				shared_ptr<Sentence> pun(new Sentence(aSen));
				punctuated.push_back(pun);
				sen_it=chara_it;
				break;
			}

		} while (chara_it!=unpun.end());
		if(chara_it==unpun.end())
			break;

	} while (sen_it!=unpun.end());

	return true;
}
