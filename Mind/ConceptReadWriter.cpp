#include "stdafx.h"
#include "ConceptReadWriter.h"

#include "FilePath.h"

#include "../CommonTools/CommonStringFunction.h"

#include "../MindDatabase/DBContainer.h"
#include "../MindDatabase/ConceptDatabase.h"

#include "../MindElement/BaseConcept.h"
#include "../MindElement/Concept.h"

namespace Mind
{
	ConceptReadWriter::ConceptReadWriter(const string dbPath):ConceptDatabase(dbPath),
		_conceptIndex(0)
	{
		SetReleaseInterval(CACHE_MONITOR_INTERVAL);
	}


	ConceptReadWriter::~ConceptReadWriter()
	{
	}

	void ConceptReadWriter::Initialize()
	{
		RunMonitor();
	}

	void ConceptReadWriter::OnMonitor()
	{
		this->ReleaseAll();
		LOG("Concepts cache is released.");
	}

	shared_ptr<BaseConcept> ConceptReadWriter::GetBaseConcept(const long baseID)
	{
		auto concept = ConceptDatabase::GetBaseConcept(baseID);
		AddConceptToCache(concept);
		return concept;
	}

	shared_ptr<BaseConcept> ConceptReadWriter::GetBaseConcept(const int id, const string word)
	{
		auto concept = ConceptDatabase::GetBaseConcept(id,word);
		AddConceptToCache(concept);
		return concept;
	}

	void ConceptReadWriter::SetCacheMonitorInterval(const int seconds)
	{
		SetReleaseInterval(seconds);
	}

	void ConceptReadWriter::AddConceptToCache(const shared_ptr<Concept> concept)
	{
		string key = GetKey(concept->GetWord()->GetString(), concept->GetId());
		Add(key, concept);
	}

	shared_ptr<Concept> ConceptReadWriter::GetNonBaseConcept(const int id, const string word)
	{
		string key = GetKey(word, id);
		auto cached = Get(key);
		if (cached != NULL)
		{
			return cached;
		}
		else
		{
			auto concept = ConceptDatabase::GetNonBaseConcept(id, word);
			AddConceptToCache(concept);
			return concept;
		}
	}

	shared_ptr<Concept> ConceptReadWriter::GetConcept(const shared_ptr<DataCollection::Word> word)
	{
		string key = GetKey(word->GetString(), 0);
		auto cached = Get(key);
		if (cached != NULL)
		{
			return cached;
		}
		else
		{
			auto concept = ConceptDatabase::GetConcept(word);
			if (concept != NULL)
			{
				AddConceptToCache(concept);
			}
			return concept;
		}
	}

	shared_ptr<Concept> ConceptReadWriter::GetConcept(const string word, const int id)
	{
		string key = GetKey(word, id);
		auto cached = Get(key);
		if (cached != NULL)
		{
			return cached;
		}
		else
		{
			auto concept = ConceptDatabase::GetConcept(word, id);
			AddConceptToCache(concept);
			return concept;
		}
	}

	shared_ptr<Concept> ConceptReadWriter::GetConcept(const string conceptID)
	{
		auto concept = ConceptDatabase::GetConcept(conceptID);
		AddConceptToCache(concept);
		return concept;
	}

	vector<shared_ptr<Concept>> ConceptReadWriter::GetConceptsWithHead(const shared_ptr<DataCollection::Character> headChara)
	{
		auto concepts = ConceptDatabase::GetConceptsWithHead(headChara);
		for (auto concept : concepts)
		{
			AddConceptToCache(concept);
		}
		return concepts;
	}

	vector<shared_ptr<Concept>> ConceptReadWriter::GetConceptsWithWord(const string word)
	{
		auto concepts = ConceptDatabase::GetConceptsWithWord(word);
		for (auto concept : concepts)
		{
			AddConceptToCache(concept);
		}
		return concepts;
	}

	vector<shared_ptr<Concept>> ConceptReadWriter::GetConceptsWithPOS(const DataCollection::PartOfSpeech pos)
	{
		auto concepts = ConceptDatabase::GetConceptsWithPOS(pos);
		for (auto concept : concepts)
		{
			AddConceptToCache(concept);
		}
		return concepts;
	}

	std::string ConceptReadWriter::GetKey(const string word, const int id) const
	{
		string key = word + "_" + CommonTool::ToString(id);
		return key;
	}

}

