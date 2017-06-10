#include "StdAfx.h"
#include "MindElementCreator.h"
#include "Concept.h"
#include "BaseConcept.h"
#include "ConceptEdge.h"
#include "ConceptChain.h"
#include "ConceptInteractTable_iConcept.h"
#include "ConceptInteractTable_Identity.h"
#include "ConceptInteractTable_MultiSet.h"
#include "ConceptLevelTable.h"

#include "../CommonTools/DBoperator.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

namespace Mind
{
	static vector<shared_ptr<Concept>> caches;

	MindElementCreator::MindElementCreator(void)
	{

	}


	MindElementCreator::~MindElementCreator(void)
	{
	}

	shared_ptr<iConcept> MindElementCreator::CreateConcept( const ElementType type ) const
	{
		switch(type)
		{
		case Host::ConceptD:
			{
				auto concept= shared_ptr<Concept>(new Concept());
				caches.push_back(concept);
				return concept;
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

	shared_ptr<iConcept> MindElementCreator::CreateConcept(const shared_ptr<DataCollection::Word> word, const ElementType type ) const
	{
		switch(type)
		{
		case Host::ConceptD:
			{
				auto concept = shared_ptr<Concept>(new Concept(word));
				caches.push_back(concept);
				return concept;
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

	shared_ptr<Concept> MindElementCreator::CreateConcept(const CommonTool::DBRow& dbRow) const
	{
		//Get common parameters.
		int id = dbRow.GetLong("id");
		string wordStr = dbRow.GetText("word");
		DataCollection::PartOfSpeech pos = (DataCollection::PartOfSpeech)dbRow.GetLong("pos");
		shared_ptr<DataCollection::Word> word = DataCollection::LanguageFunc::GetParticularWord(wordStr, pos);

		shared_ptr<Concept> concept;
		//Check type of row.
		if (dbRow.HasColumn("baseID"))
		{
			//It is base concept.

			auto baseID = dbRow.GetLong("baseID");
			concept = CreateBaseConcept(word, id, baseID);
		}
		else
		{
			//It is non base concept.

			concept = make_shared<Concept>(word);
			concept->SetId(id);
		}
		caches.push_back(concept);

		return concept;
	}


	shared_ptr<BaseConcept> MindElementCreator::CreateBaseConcept(const shared_ptr<DataCollection::Word> word, const int conceptID, const int baseID) const
	{
		shared_ptr<BaseConcept> newConcept = make_shared<BaseConcept>(word);
		newConcept->SetId(conceptID);
		newConcept->SetBaseId(baseID);

		caches.push_back(newConcept);
		return newConcept;
	}

	shared_ptr<BaseConcept> MindElementCreator::CreateBaseConcept(const CommonTool::DBRow& dbRow) const
	{
		long baseID = dbRow.GetLong("baseID");
		int id = dbRow.GetLong("id");
		string wordStr = dbRow.GetText("word");
		auto pos = (DataCollection::PartOfSpeech)dbRow.GetLong("pos");

		shared_ptr<DataCollection::Word> word = DataCollection::LanguageFunc::GetParticularWord(wordStr, pos);

		shared_ptr<BaseConcept> newConcept = make_shared<BaseConcept>(word);
		newConcept->SetId(id);
		newConcept->SetBaseId(baseID);

		caches.push_back(newConcept);

		return newConcept;
	}

	shared_ptr<iConceptChain> MindElementCreator::CreateConceptChain(const ElementType type) const
	{
		switch(type)
		{
		case Host::ConceptChainD:
			{
				return shared_ptr<ConceptChain>(new ConceptChain());
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

	shared_ptr<iConceptChain> MindElementCreator::CreateConceptChain( const vector<shared_ptr<iConcept>>& val,const ElementType type ) const
	{
		switch(type)
		{
		case Host::ConceptChainD:
			{
				return shared_ptr<ConceptChain>(new ConceptChain(val));
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

	shared_ptr<iConceptEdge> MindElementCreator::CreateConceptEdge( const ElementType type ) const
	{
		switch(type)
		{
		case Host::ConceptEdgeD:
			{
				return shared_ptr<ConceptEdge>(new ConceptEdge());
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

	shared_ptr<iConceptInteractTable> MindElementCreator::CreateConceptInteractTable(const ElementType type) const
	{
		switch (type)
		{
		case Host::ConceptInteractTable_ConceptBased:
		{
			return shared_ptr<ConceptInteractTable_iConcept>(new ConceptInteractTable_iConcept());
			break;
		}
		case Host::ConceptInteractTable_IdentityBased:
		{
			return shared_ptr<ConceptInteractTable_Identity>(new ConceptInteractTable_Identity());
			break;
		}
		case Host::ConceptInteractTable_SetBased:
		{
			return shared_ptr<ConceptInteractTable_MultiSet>(new ConceptInteractTable_MultiSet());
			break;
		}
		default:
		{
			return NULL;
		}
		}
	}

	shared_ptr<iConceptInteractTable> MindElementCreator::CreateConceptInteractTable(const vector<MindType::ConceptPair>& pairs,const ElementType type) const
	{
		shared_ptr<iConceptInteractTable> res = CreateConceptInteractTable(type);

		if(res!=NULL)
		{
			for (size_t i=0;i<pairs.size();++i)
			{
				res->Add(pairs[i].first,pairs[i].second);
			}
		}

		return res;
	}

	shared_ptr<iConceptLevelTable> MindElementCreator::CreateConceptLevelTable(const shared_ptr<iConcept> val ,const  ElementType type ) const
	{
		switch(type)
		{
		case Host::ConceptLevelTableD:
			{
				return shared_ptr<ConceptLevelTable>(new ConceptLevelTable(val));
				break;
			}
		default:
			{
				return NULL;
			}
		}
	}

}

