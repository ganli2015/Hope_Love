#pragma once
#include "InOut.h"
#include "../MindInterface/iMindElementCreator.h"

namespace Mind
{
	class BaseConcept;

	class _MINDELEMENTINOUT MindElementCreator : public iMindElementCreatorImp
	{
	public:
		MindElementCreator(void);
		~MindElementCreator(void);

		virtual shared_ptr<iConcept> CreateConcept(const ElementType type) const ;
		virtual shared_ptr<iConcept> CreateConcept(const shared_ptr<DataCollection::Word> word,const ElementType type) const ;
		//////////////////////////////////////////////////////////////////////////
		//conceptID: ID for concept with the same word.
		//baseID: ID fro base concept.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<BaseConcept> CreateBaseConcept(const shared_ptr<DataCollection::Word> word,const int conceptID,const int baseID) const ;

		virtual shared_ptr<iConceptChain> CreateConceptChain(const ElementType type) const ;
		virtual shared_ptr<iConceptChain> CreateConceptChain(const vector<shared_ptr<iConcept>>& val,const ElementType type) const ;

		virtual shared_ptr<iConceptEdge> CreateConceptEdge(const ElementType type) const ;

		virtual shared_ptr<iConceptInteractTable> CreateConceptInteractTable(const ElementType type) const ;
		virtual shared_ptr<iConceptInteractTable> CreateConceptInteractTable(const vector<MindType::ConceptPair>& pairs,const ElementType type) const;

		virtual shared_ptr<iConceptLevelTable> CreateConceptLevelTable(const shared_ptr<iConcept> val ,const ElementType type) const ;
	
	private:

	};
}


