#include "StdAfx.h"
#include "Sentence.h"
#include "Character.h"

#include "LanguageFunc.h"
#include "Word.h"

using namespace CommonTool;
using namespace std;
namespace DataCollection
{
	Sentence::Sentence(void)
	{
	}


	Sentence::~Sentence(void)
	{
// 		DeleteVectorPtr(_rawSentence);
// 		for (size_t i=0;i<_segmented.size();++i)
// 		{
// 			DeleteVectorPtr(_segmented[i]);
// 		}
		
	}

	Sentence::Sentence(std::vector<shared_ptr<Character>> val):_raw(val)
	{

	}

	Sentence::Sentence(std::string str)
	{
		_raw=LanguageFunc::ConvertStringToCharacter(str);
	}

	std::vector<shared_ptr<Character>> Sentence::GetRawSentence() const
	{
		return _raw;
	}

	void Sentence::SetGrammard(const std::vector<shared_ptr<DataCollection::Word>> vec)
	{
		//StructuredSentence use the same words as well as GrammardSentence.
		_grammard=shared_ptr<GrammardSentence>(new GrammardSentence(vec));
		_structured=shared_ptr<StructuredSentence>(new StructuredSentence(vec));
	}

	std::vector<shared_ptr<Word>> Sentence::GetGrammard(  ) const
	{
		if(_grammard==NULL)
		{
			return vector<shared_ptr<Word>>();
		}
		else
		{
			return _grammard->Get();
		}
	}

	void Sentence::BuildGrammarAssociation( const vector<GrammarPattern>& patterns )
	{
		if(_grammard!=NULL)
		{
			_grammard->BuildGrammarAssociation(patterns);
		}
		else
		{
			return ;
		}
	}

	void Sentence::GetAssociationInfo( const int i_thWord,vector<vector<int>>& associatedIndexes,vector<GrammarPattern>& associatedPatterns )
	{
		if(_grammard!=NULL)
		{
			_grammard->GetAssociationInfo(i_thWord,associatedIndexes,associatedPatterns);

		}
		else
		{
			return ;
		}
	}

	size_t Sentence::GrammarWordCount() const
	{
		if(_grammard!=NULL)
		{
			return _grammard->WordCount();

		}
		else
		{
			return 0;
		}
	}

	void Sentence::SetWordIntensity( const size_t i_word,const size_t j_word,double intensity )
	{
		if(_structured!=NULL)
		{
			_structured->SetIntensity(i_word,j_word,intensity);
		}
		else
		{
			return ;
		}
	}

	double Sentence::GetWordIntensity( const size_t i_word,const size_t j_word )
	{
		if(_structured!=NULL)
		{
			return _structured->GetIntensity(i_word,j_word);
		}
		else
		{
			return -1;
		}
	}

	std::string Sentence::GetString() const
	{
		return LanguageFunc::ConvertCharacterToString(_raw);
	}

	void Sentence::AddSubSentence( const std::vector<shared_ptr<DataCollection::Character>> vec )
	{
		shared_ptr<SubSentence> subsentence(new SubSentence(vec));
		SubSentenceInfo subInfo(LanguageFunc::ConvertCharacterToString(vec),subsentence);

		_subInfos.push_back(subInfo);
	}

	void Sentence::AddSubSentence( const string str )
	{
		shared_ptr<SubSentence> subsentence(new SubSentence(str));
		SubSentenceInfo subInfo(str,subsentence);

		_subInfos.push_back(subInfo);
	}

	size_t Sentence::Count_SubSentence() const
	{
		return _subInfos.size();
	}

	std::string Sentence::GetSubSentence( const size_t i ) const
	{
		return _subInfos[i].GetSentence()->GetString();
	}

	bool Sentence::SearchSubInfo( const string str,int& infoIndex ) const
	{
		for (size_t i=0;i<_subInfos.size();++i)
		{
			if(_subInfos[i].GetString()==str)
			{
				infoIndex=i;
				return true;
			}
		}

		return false;
	}

	shared_ptr<Word> Sentence::GetGrammardWord( const size_t i ) const
	{
		if(i>GrammarWordCount())
		{
			return NULL;
		}
		else
		{
			return GetGrammard()[i];
		}
	}


	SubSentence::SubSentence()
	{

	}

	SubSentence::~SubSentence()
	{

	}

	SubSentence::SubSentence(std::vector<shared_ptr<Character>> vec)
	{
		_raw=vec;
	}

	SubSentence::SubSentence(std::string str)
	{
		std::vector<shared_ptr<Character>> val=LanguageFunc::ConvertStringToCharacter(str);
		_raw=val;
	}

	std::vector<shared_ptr<Character>> SubSentence::GetRawSentence() const
	{
		return _raw;
	}

	std::string SubSentence::GetString() const
	{
		return LanguageFunc::ConvertCharacterToString(_raw);
	}



	SegmentedSentence::SegmentedSentence()
	{

	}

	SegmentedSentence::~SegmentedSentence()
	{

	}

	SegmentedSentence::SegmentedSentence(const std::vector<shared_ptr<DataCollection::Word>> vec)
	{
		_seg=vec;
	}

	std::vector<shared_ptr<Word>> SegmentedSentence::Get() const
	{
		return _seg;
	}

	std::string SegmentedSentence::GetString() const
	{
		string res = "";
		if (_seg.empty()) return res;

		res += _seg.front()->GetString();
		for (size_t i=1;i<_seg.size();++i)
		{
			res += "/" + _seg[i]->GetString();
		}

		return res;
	}

}


