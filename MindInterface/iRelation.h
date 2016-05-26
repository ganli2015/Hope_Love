#pragma once
#include "InOut.h"
#include "PublicTypedef.h"

namespace Mind
{
	class iConcept;
	class iConceptInteractTable;
}

namespace LogicSystem
{
	class iExpression;
	template<class T>
	class iSymbol;
	class iRelationConstraint;

	///iRelation indicates the logic relationship between logic symbols.
	class _MINDINTERFACEINOUT iRelation: public MyObject
	{
	protected:
		struct PairInfo;
		typedef MindType::ConceptPair ConceptPair;
		typedef LogicType::ConSymbol ConSymbol;
		typedef LogicType::SymbolPair SymbolPair;
		typedef vector<iRelation::PairInfo> PairSequence;


		//Store concept pair and related symbol pair.
		struct PairInfo
		{
			ConceptPair cPair;
			SymbolPair sPair;
		};

		iRelation::PairSequence _satisfiedSequence;

	public:
		iRelation(void);
		virtual ~iRelation(void);

		virtual string GetString() const =0;

		///Add Constraints to symbols.
		virtual void AddConstraint(const shared_ptr<iRelationConstraint> constraint) =0;

		///Check whether <expre> satisfy the relation <me>.
		virtual bool Satisfy(const shared_ptr<iExpression> expre) =0;

		///Find match symbol and concept pairs in <conceptPairs>.
		virtual vector<PairSequence> FindMatchedPairSequence(const vector<ConceptPair>& conceptPairs) const =0;

		///Generate iRelation in which all of symbols refer to a definite object according to symbols of <me>.
		///If one of symbols refers to null object, then iRelation will be null.
		virtual shared_ptr<iRelation> GenerateSpecialRelation() const =0;

		///Use <Satisfy> before!
		///After <Satisfy>, symbols of <me> will refer to concepts and then generate a special relation of <relation>.
		///Each symbol in the result relation will refer to a definite object.
		///Same symbols in <relation> and <me> will refer to the same objects.
		shared_ptr<iRelation> SymbolResonance(const shared_ptr<iRelation> relation) const ;

		///Generate the corresponding iConceptInteractTable according to symbol pairs of <me>.
		///If one symbol pair both refer to certain concepts, then the pair will appear in iConceptInteractTable.
		virtual shared_ptr<Mind::iConceptInteractTable> GenerateConceptTable() const =0;
	protected:
		bool SatifyConstraint(const vector<PairInfo>& pairInfos,const vector<shared_ptr<iRelationConstraint>>& constraints) const;
		void RemoveSequencesUnsatifyConstraints(const vector<shared_ptr<iRelationConstraint>>& constraints,vector<iRelation::PairSequence>& sequences) const;

		///Symbols in <pairInfos> will be bind to concepts and you can do anything after using class <doSth>.
		///<doSth> should have a member function of <Do> to execute your computation.
		///Before leave the function ,the bind objects will be released. 
		template<class DoSth>
		void SymbolBindAndRelease(const PairSequence& pairInfos,DoSth& doSth) const
		{
			BindObjects(pairInfos);
			doSth.Do();
			ReleaseObjects(pairInfos);
		}

	private:
		void BindObjects(const PairSequence& pairInfos) const;
		void ReleaseObjects(const PairSequence& pairInfos) const;
	};

	///iRelationNode contains the several sub relations and their relationship is And or Or.
	class _MINDINTERFACEINOUT iRelationNode : public iRelation
	{
	public:
		enum State
		{
			And,
			Or
		};

	protected:
		State _state;
		vector<shared_ptr<iRelation>> _subRelations;

	public:
		iRelationNode(void);
		virtual ~iRelationNode(void);

		void SetState(const State s){_state=s;}
		void AddSubRelation(const shared_ptr<iRelation> condition){_subRelations.push_back(condition);}
		virtual void AddConstraint(const shared_ptr<iRelationConstraint> constraint) =0;

		virtual string GetString() const =0;
		virtual bool Satisfy(const shared_ptr<iExpression> expre) =0;

	protected:
	};

	///An iRelationLeaf contains the relationship between several logic symbols.
	class _MINDINTERFACEINOUT iRelationLeaf : public iRelation
	{
	public:
		iRelationLeaf(void);
		virtual ~iRelationLeaf();

		///Add the relation of the two symbols to <me>.
		///<from> acts on <to>.
		virtual void AddRelation(const shared_ptr<ConSymbol> from,const shared_ptr<ConSymbol> to) =0;
		///Add Constraints to symbols.
		virtual void AddConstraint(const shared_ptr<iRelationConstraint> constraint) =0;
		virtual string GetString() const =0;
		virtual bool Satisfy(const shared_ptr<iExpression> expre) =0;
	};


}

