#pragma once
#include "MindDatabase.h"

namespace CommonTool
{
	class DBoperator;
	class DBRow;
	class QueryStatement;
}

namespace DataCollection
{
	enum PartOfSpeech;
	class Word;
	class Character;
}

namespace Mind
{
	class BaseConcept;
	class Concept;

	//////////////////////////////////////////////////////////////////////////
	//Database for Base Concept, NonBase Concept and Concept Connection.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINOUT ConceptDatabase : public MindDatabase
	{
		const string BaseConceptTable;
		const string NonBaseConceptTable;

		vector<string> _tables;

		friend class DataPrepareForHopeLove;
	public:
		ConceptDatabase();
		~ConceptDatabase();

		void AddBaseConcept(const shared_ptr<BaseConcept> concept);
		//////////////////////////////////////////////////////////////////////////
		//Add base concept.
		//ID and base ID will be automatically computed.
		//////////////////////////////////////////////////////////////////////////
		void AddBaseConcept(const shared_ptr<DataCollection::Word> word);
		//////////////////////////////////////////////////////////////////////////
		//Add non base concept.
		//ID will be automatically computed.
		//////////////////////////////////////////////////////////////////////////
		void AddNonBaseConcept(const shared_ptr<DataCollection::Word> word);
		void AddNonBaseConcept(const shared_ptr<DataCollection::Word> word,const int id);

		//////////////////////////////////////////////////////////////////////////
		//<index> is the index in database.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<BaseConcept> GetBaseConcept(const long index);
		//////////////////////////////////////////////////////////////////////////
		//<id> is the ID of concepts with same words.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<Concept> GetNonBaseConcept(const int id,const string word);
		//////////////////////////////////////////////////////////////////////////
		//Get the first concept with the same word.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<Concept> GetConcept(const shared_ptr<DataCollection::Word> word);
		//////////////////////////////////////////////////////////////////////////
		//Get the first concept with the same word string and id.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<Concept> GetConcept(const string word,const int id);

		//////////////////////////////////////////////////////////////////////////
		//Get total count of base concept.
		//////////////////////////////////////////////////////////////////////////
		size_t GetBaseConceptCount();

		//////////////////////////////////////////////////////////////////////////
		//Check if there is any concept with word <word>.
		//////////////////////////////////////////////////////////////////////////
		bool HasWord(const shared_ptr<DataCollection::Word> word);
		//////////////////////////////////////////////////////////////////////////
		//Check if there is any concept with string <wordStr>.
		//////////////////////////////////////////////////////////////////////////
		bool HasString(const string wordStr);

		//////////////////////////////////////////////////////////////////////////
		//Get all concepts whose word starts with <headChara>.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<Concept>> GetConceptsWithHead(const shared_ptr<DataCollection::Character> headChara);

		//////////////////////////////////////////////////////////////////////////
		//Get all concepts whose word string is <word>.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<Concept>> GetConceptsWithWord(const string word);
		//////////////////////////////////////////////////////////////////////////
		//Get all concepts whose POS is <pos>.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<Concept>> GetConceptsWithPOS(const DataCollection::PartOfSpeech pos);

	private:
		vector<CommonTool::DBRow> QueryRows(const CommonTool::QueryStatement& cmd);
		vector<CommonTool::DBRow> QueryRows(const string& cmd);

		void AddBaseConcept(const long index, const int id, const string word, const DataCollection::PartOfSpeech pos);
		void AddNonBaseConcept(const int id, const string word, const DataCollection::PartOfSpeech pos);
		CommonTool::DBRow GetBaseConceptRow(const long index);
		CommonTool::DBRow GetNonBaseConceptRow(const int id, const string word);

		vector<CommonTool::DBRow> GetRowsWithWord(const string word);

		vector<CommonTool::DBRow> QueryForTables(const vector<CommonTool::QueryStatement>& statements);

		vector<CommonTool::QueryStatement> CreateQryForTables() const;


		vector<shared_ptr<BaseConcept>> GetAllBaseConcepts();
		vector<shared_ptr<Concept>> GetAllNonBaseConcepts();

		string GenerateConceptPrimaryKey(const string word, const int id);

		//////////////////////////////////////////////////////////////////////////
		//Change primary key of each row to a hash value computed from word and id.
		//////////////////////////////////////////////////////////////////////////
		void ChangePrimaryKeyToHash();
	};
}


