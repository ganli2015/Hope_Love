#pragma once
#include "InOut.h"

#include "../CommonTools/NonCopyable.h"
#include "../CommonTools/CacheManager.h"

#include "../MindDatabase/ConceptDatabase.h"

namespace Mind
{
	class ConceptDatabase;
	class ConceptCacheManager;
	class Concept;

	//Entry for reading and writing Concepts.
	//It is a mixture of Database and Cache, and behaves more like a database.
	class _MINDTESTEXPORT ConceptReadWriter : 
		protected CommonTool::CacheManager<Concept>,
		public Mind::ConceptDatabase
	{
		//Default interval.(seconds)
		static const int CACHE_MONITOR_INTERVAL = 10 * 60;

		long _conceptIndex;
	public:
		ConceptReadWriter(const string dbPath);
		virtual ~ConceptReadWriter();

		void Initialize();
		//Set interval before Initialize.
		void SetCacheMonitorInterval(const int seconds);

		/*****Override functions of ConceptDatabase*****/

		//<baseID> is the baseID in database.
		virtual shared_ptr<BaseConcept> GetBaseConcept(const long baseID);
		//<baseID> is the baseID in database.
		virtual shared_ptr<BaseConcept> GetBaseConcept(const int id, const string word);
		//<id> is the ID of concepts with same words.
		virtual shared_ptr<Concept> GetNonBaseConcept(const int id, const string word);
		//Get the first concept with the same word.
		virtual shared_ptr<Concept> GetConcept(const shared_ptr<DataCollection::Word> word);
		//Get the first concept with the same word string and id.
		virtual shared_ptr<Concept> GetConcept(const string word, const int id);
		//Get the first concept with conceptID.
		virtual shared_ptr<Concept> GetConcept(const string conceptID);

		//Get all concepts whose word starts with <headChara>.
		virtual vector<shared_ptr<Concept>> GetConceptsWithHead(const shared_ptr<DataCollection::Character> headChara);

		//Get all concepts whose word string is <word>.
		virtual vector<shared_ptr<Concept>> GetConceptsWithWord(const string word);
		//Get all concepts whose POS is <pos>.
		virtual vector<shared_ptr<Concept>> GetConceptsWithPOS(const DataCollection::PartOfSpeech pos);

		/**********************************************/

	private:
		virtual void OnMonitor();

		void AddConceptToCache(const shared_ptr<Concept> concept);

		string GetKey(const string word, const int id) const;
	};
}


