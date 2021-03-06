#include "StdAfx.h"
#include "Cerebrum.h"
#include "ConceptSet.h"
#include "GrammaSet.h"
#include "WordRelationTable.h"
#include "ConceptInteractTableContainer.h"
#include "ConceptReactSystem.h"

#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/Character.h"

#include "../MindInterface/iLogicKnowledge.h"

using namespace DataCollection;

namespace Mind
{
	Cerebrum* Cerebrum::_instance=NULL;
	Cerebrum::Cerebrum(void):
	_conceptset(new ConceptSet),
		_grammaset(new GrammarSet),
		_conceptInteractTableContainer(new ConceptInteractTableContainer)
	{
		_conceptReactSystem=(new ConceptReactSystem(_conceptset));
		_grammaset->InitializeGrammarModel();
	}

	Cerebrum::~Cerebrum()
	{
		delete _conceptReactSystem;
		delete _conceptInteractTableContainer;
		delete _grammaset;
		delete _conceptset;
		if(_logicKnowledge!=NULL)
		{
			delete _logicKnowledge;
		}
		_instance=NULL;
	}

	void Cerebrum::PostInitialize()
	{
		_logicKnowledge=NULL;
	}

	Cerebrum* Cerebrum::Instance()
	{
		if(_instance==NULL)
		{
			_instance=new Cerebrum;
			_instance->PostInitialize();
			LOG("Initialized Cereburm.");
		}
		return _instance;
	}

	bool Cerebrum::IsInMind( const std::string str ) const
	{
		return _conceptset->IsConceptExist(str);
	}

	bool Cerebrum::IsInMind( const shared_ptr<DataCollection::Word> word ) const
	{
		return IsInMind(word->GetString());
	}

	std::vector<shared_ptr<DataCollection::Word>> Cerebrum::GetAllKindsofWord( const shared_ptr<DataCollection::Word> word ) const
	{
		return _conceptset->GetAllKindsofWord(word);
	}

	std::vector<DataCollection::GrammarPattern> Cerebrum::ContainSubsequence( const DataCollection::GrammarPattern& pattern ) const
	{
		return _grammaset->ContainSubsequence(pattern);
	}

	int Cerebrum::CountOfSubsequence( const DataCollection::GrammarPattern& pattern ) const
	{
		return _grammaset->CountOfSubsequence(pattern);
	}

	int Cerebrum::GetFreqencyofPattern( const DataCollection::GrammarPattern& pattern ) const
	{
		return _grammaset->GetFreqencyofPattern(pattern);
	}

	void Cerebrum::IncreasePatternFreqency( const DataCollection::GrammarPattern& pattern )
	{
		_grammaset->IncreasePatternFreqency(pattern);
	}

	void Cerebrum::MakeConceptConnection( const shared_ptr<DataCollection::Word> from,const shared_ptr<DataCollection::Word> to )
	{
		return _conceptset->MakeConnection(from,to);
	}

// 	std::vector<pair<shared_ptr<Concept>,shared_ptr<Concept>>> Cerebrum::GetAllInteractPairs() const
// 	{
// 		std::vector<pair<shared_ptr<Concept>,shared_ptr<Concept>>> res;
// 		if(_conceptInteractTableContainer==NULL)
// 		{
// 			return res;
// 		}
// 		else
// 		{
// 			return _conceptInteractTableContainer->GetAllInteractPairs();
// 		}
// 	}

// 	void Cerebrum::ClearConceptInteractTable()
// 	{
// 		_conceptInteractTableContainer=new ConceptInteractTableContainer;
// 	}

	vector<DataCollection::GrammarPattern> Cerebrum::GrammarPatternSortByFrequency() const
	{
		return _grammaset->GrammarPatternSortByFrequency();
	}

	std::vector<DataCollection::GrammarPattern> Cerebrum::ContainedSubsequence( const DataCollection::GrammarPattern& pattern ) const
	{
		return _grammaset->ContainedSubsequence(pattern);
	}

	vector<shared_ptr<DataCollection::Word>> Cerebrum::GetAllWordsOfPOS( const DataCollection::PartOfSpeech pos ) const
	{
		return _conceptset->GetAllWordsOfPOS(pos);
	}

	int Cerebrum::MaxLength_WordWithHead( const shared_ptr<DataCollection::Character> headChara ) const
	{
		return _conceptset->MaxLength_WordWithHead(headChara);
	}

	shared_ptr<iConcept> Cerebrum::GetConcept( const shared_ptr<DataCollection::Word> word ) const
	{
		return _conceptset->GetConceptPtr(word);
	}

	shared_ptr<iConcept> Cerebrum::GetConcept( const Identity identity ) const
	{
		return _conceptset->GetConceptPtr(identity);
	}

// 	void Cerebrum::BuildConceptInteractTable( const shared_ptr<Concept> fromConcept,const shared_ptr<Concept> toConcept )
// 	{
// 		if(fromConcept==NULL || toConcept==NULL)
// 		{
// 			return ;
// 		}
// 
// 		shared_ptr<ConceptInteractTable> conceptTable=_conceptset->GetDeepInteractTable(fromConcept,toConcept);
// 		if(_conceptInteractTableContainer==NULL)
// 		{
// 			_conceptInteractTableContainer=new ConceptInteractTableContainer;
// 		}
// 		_conceptInteractTableContainer->Add(conceptTable,fromConcept,toConcept);
// 	}

	vector<ConceptChainProperty> Cerebrum::React( const shared_ptr<iConceptChain> chain )
	{
		return _conceptReactSystem->React(chain);
	}

	vector<shared_ptr<iConcept>> Cerebrum::SearchForwardConcepts( const shared_ptr<iConcept> concept ) const
	{
		return _conceptset->SearchForwardConcepts(concept);
	}

	vector<shared_ptr<iConcept>> Cerebrum::SearchBackwardConcepts( const shared_ptr<iConcept> concept ) const
	{
		return _conceptset->SearchBackwardConcepts(concept);
	}

	double Cerebrum::GetP_Forward( const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& forward ) const
	{
		return _grammaset->GetP_Forward(me,forward);
	}

	double Cerebrum::GetP_Backward( const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& backward ) const
	{
		return _grammaset->GetP_Backward(me,backward);
	}

	double Cerebrum::ComputePossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return _grammaset->ComputePossibility(sentence);
	}

	double Cerebrum::ComputeLocalPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return _grammaset->ComputeP_GrammarLocalAnalysis(sentence);
	}

	double Cerebrum::ComputeGrammarPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return _grammaset->ComputeGrammarPossibility(sentence);
	}

	map<double, DataCollection::PartOfSpeech> Cerebrum::ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const
	{
		return _grammaset->ComputePossibilityTable(forwardPos, backwardPos);
	}

	vector<shared_ptr<LogicSystem::iDeduceResult>> Cerebrum::Deduce(const shared_ptr<LogicSystem::iExpression> expre) const
	{
		return _logicKnowledge->Deduce(expre);
	}

	vector<shared_ptr<LogicSystem::iDeduceResult>> Cerebrum::Deduce( const shared_ptr<iConceptInteractTable> expre ) const
	{
		return _logicKnowledge->Deduce(expre);
	}

	void Cerebrum::AddLogicStatement( const shared_ptr<LogicSystem::iLogicStatement> statement )
	{
		_logicKnowledge->Add(statement);
	}

	vector<shared_ptr<iConcept>> Cerebrum::FindConceptWithMatchedDisc( const shared_ptr<iConceptInteractTable> description ) const
	{
		return _conceptset->FindConceptWithMatchedDisc(description);
	}

	void Cerebrum::FindConceptWithMatchedDisc( const shared_ptr<iConceptInteractTable> description, vector<DescMatchedConceptInfo>& matchedInfos ) const
	{
		_conceptset->FindConceptWithMatchedDisc(description,matchedInfos);
	}

	int Cerebrum::BaseConceptCount() const
	{
		return _conceptset->BaseConceptCount();
	}

}



