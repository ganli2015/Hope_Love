#pragma once
#include "InOut.h"
#include "../MindInterface/iCerebrum.h"


namespace DataCollection
{
	class Character;
	class Word;
	class GrammarPattern;
	class WordRelationTable;
	enum PartOfSpeech;
}

namespace LogicSystem
{
	class iLogicKnowledge;
}

namespace Mind
{
	class iConcept;
	class ConceptSet;
	class GrammarSet;
	class ConceptInteractTableContainer;
	class ConceptReactSystem;
	class iConceptChain;
	struct Identity;
	struct ConceptChainProperty;

	class _MINDINOUT Cerebrum : public iCerebrum
	{
		static Cerebrum* _instance;
		
		ConceptSet* _conceptset;
		GrammarSet* _grammaset;
		ConceptInteractTableContainer* _conceptInteractTableContainer;
		ConceptReactSystem* _conceptReactSystem;
		LogicSystem::iLogicKnowledge* _logicKnowledge;

		Cerebrum(void);
		void PostInitialize();
	public:
		static Cerebrum* Instance();
		virtual ~Cerebrum();

		void SetLogicKnowledge(LogicSystem::iLogicKnowledge* val){_logicKnowledge=val;}

		//Concept Related Functions
		virtual bool IsInMind(const std::string str) const;
		virtual bool IsInMind(const shared_ptr<DataCollection::Word> word) const;

		int MaxLength_WordWithHead(const shared_ptr<DataCollection::Character> headChara) const;

		//得到已知的所有词性的<word>。
		virtual std::vector<shared_ptr<DataCollection::Word>> GetAllKindsofWord(const shared_ptr<DataCollection::Word> word) const;
		virtual vector<shared_ptr<DataCollection::Word>> GetAllWordsOfPOS(const DataCollection::PartOfSpeech pos) const;

		void MakeConceptConnection(const shared_ptr<DataCollection::Word> from,const shared_ptr<DataCollection::Word> to);
		virtual shared_ptr<iConcept> GetConcept(const shared_ptr<DataCollection::Word> word) const ;
		virtual shared_ptr<iConcept> GetConcept(const Identity identity) const;
		virtual vector<shared_ptr<iConcept>> SearchForwardConcepts(const shared_ptr<iConcept> concept) const;
		virtual vector<shared_ptr<iConcept>> SearchBackwardConcepts(const shared_ptr<iConcept> concept) const;

		virtual vector<shared_ptr<iConcept>> FindConceptWithMatchedDisc(const shared_ptr<iConceptInteractTable> description) const ;
		virtual void FindConceptWithMatchedDisc(const shared_ptr<iConceptInteractTable> description, vector<DescMatchedConceptInfo>& matchedInfos) const;

		virtual int BaseConceptCount() const ;

		virtual vector<shared_ptr<iConcept>> GetAllBaseConcepts() const ;

		virtual vector<shared_ptr<iConcept>> GetAllConcepts() const ;

		virtual vector<shared_ptr<iConcept>> GetConcepts(const string str) const ;

		//Grammar Related Functions
		//搜索<pattern>所包含的子pattern.
		virtual std::vector<DataCollection::GrammarPattern> ContainSubsequence(const DataCollection::GrammarPattern& pattern) const;
		//搜索包含<pattern>的pattern.
		virtual std::vector<DataCollection::GrammarPattern> ContainedSubsequence(const DataCollection::GrammarPattern& pattern) const;
		virtual int CountOfSubsequence(const DataCollection::GrammarPattern& pattern) const;
		virtual int GetFreqencyofPattern(const DataCollection::GrammarPattern& pattern) const;
		virtual void IncreasePatternFreqency(const DataCollection::GrammarPattern& pattern);
		vector<DataCollection::GrammarPattern> GrammarPatternSortByFrequency() const ;
		//获得me的后一个词性是forward的概率.
		virtual double GetP_Forward(const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& forward) const;
		//获得me的前一个词性是backward的概率.
		virtual double GetP_Backward(const DataCollection::PartOfSpeech& me,const DataCollection::PartOfSpeech& backward) const;

		virtual double ComputePossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
		virtual double ComputeLocalPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const;
		virtual double ComputeGrammarPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const ;

		//Compute Possibility table of all POS in the condition that its forward POS is <forwardPos> and its backward POS is <backwardPos>.
		map<double,DataCollection::PartOfSpeech> ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const;

		//Concept React System Functions
		virtual vector<ConceptChainProperty> React(const shared_ptr<iConceptChain> chain);

		//LogicKnowledge related functions
		virtual vector<shared_ptr<LogicSystem::iDeduceResult>> Deduce(const shared_ptr<LogicSystem::iExpression> expre) const;
		virtual vector<shared_ptr<LogicSystem::iDeduceResult>> Deduce(const shared_ptr<iConceptInteractTable> conceptTable) const ;
		virtual void AddLogicStatement(const shared_ptr<LogicSystem::iLogicStatement> statement) ;

	};
}


