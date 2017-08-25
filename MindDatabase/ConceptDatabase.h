#pragma once
#include "MindDatabase.h"

namespace CommonTool
{
	class DBoperator;
	class DBRow;
	class QueryStatement;
	class UpdateStatement;
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
	class iConceptInteractTable;
	class iConcept;

	//////////////////////////////////////////////////////////////////////////
	//Database for Base Concept, NonBase Concept and Concept Connection.
	//////////////////////////////////////////////////////////////////////////
	class _MINDDATABASEINOUT ConceptDatabase : public MindDatabase
	{
		const string BaseConceptTable;
		const string NonBaseConceptTable;
		const string ConceptConnectionTable;

		vector<string> _tables;

		friend class DataPrepareForHopeLove;
	public:
		ConceptDatabase(const string dbPath);
		virtual ~ConceptDatabase();

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
		void AddNonBaseConcept(const shared_ptr<Concept> concept);

		void DeleteBaseConcept(const string word, const int conceptId);
		void DeleteNonBaseConcept(const string word, const int id);

		//////////////////////////////////////////////////////////////////////////
		//Add connection between two concepts.
		//////////////////////////////////////////////////////////////////////////
		void AddConnection(const shared_ptr<DataCollection::Word> fromWord, const int fromId,
			const shared_ptr<DataCollection::Word> toWord, const int toId);

		//////////////////////////////////////////////////////////////////////////
		//Add Modification between connection of two concepts.
		//The modification is represented by <modTable>.
		//////////////////////////////////////////////////////////////////////////
		void AddModification(const shared_ptr<DataCollection::Word> fromWord, const int fromId,
			const shared_ptr<DataCollection::Word> toWord, const int toId,
			const shared_ptr<iConceptInteractTable> modTable);

		//////////////////////////////////////////////////////////////////////////
		//<baseID> is the baseID in database.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<BaseConcept> GetBaseConcept(const long baseID);
		//////////////////////////////////////////////////////////////////////////
		//<baseID> is the baseID in database.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<BaseConcept> GetBaseConcept(const int id, const string word);
		//////////////////////////////////////////////////////////////////////////
		//<id> is the ID of concepts with same words.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Concept> GetNonBaseConcept(const int id,const string word);
		//////////////////////////////////////////////////////////////////////////
		//Get the first concept with the same word.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Concept> GetConcept(const shared_ptr<DataCollection::Word> word);
		//////////////////////////////////////////////////////////////////////////
		//Get the first concept with the same word string and id.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Concept> GetConcept(const string word,const int id);
		//////////////////////////////////////////////////////////////////////////
		//Get the first concept with conceptID.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Concept> GetConcept(const string conceptID);

		vector<shared_ptr<Concept>> GetAllNonBaseConcepts();
		vector<shared_ptr<BaseConcept>> GetAllBaseConcepts();

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
		virtual vector<shared_ptr<Concept>> GetConceptsWithHead(const shared_ptr<DataCollection::Character> headChara);

		//////////////////////////////////////////////////////////////////////////
		//Get all concepts whose word string is <word>.
		//////////////////////////////////////////////////////////////////////////
		virtual vector<shared_ptr<Concept>> GetConceptsWithWord(const string word);
		//////////////////////////////////////////////////////////////////////////
		//Get all concepts whose POS is <pos>.
		//////////////////////////////////////////////////////////////////////////
		virtual vector<shared_ptr<Concept>> GetConceptsWithPOS(const DataCollection::PartOfSpeech pos);

	private:


		void AddBaseConcept(const long index, const int id, const string word, const DataCollection::PartOfSpeech pos);
		void AddNonBaseConcept(const int id, const string word, const DataCollection::PartOfSpeech pos);
		CommonTool::DBRow GetBaseConceptRow(const long index);
		CommonTool::DBRow GetNonBaseConceptRow(const int id, const string word);

		//////////////////////////////////////////////////////////////////////////
		//Get rows from concept table.
		//////////////////////////////////////////////////////////////////////////
		vector<CommonTool::DBRow> GetRowsWithWord(const string word);

		//////////////////////////////////////////////////////////////////////////
		//Query for both NonBaseConcept table and BaseConceptTable.
		//////////////////////////////////////////////////////////////////////////
		CommonTool::DBRow GetRowInConceptTables(const string pk);

		vector<CommonTool::DBRow> QueryForTables(const vector<CommonTool::QueryStatement>& statements);

		vector<CommonTool::QueryStatement> CreateQryForTables() const;

		string GenerateConceptPrimaryKey(const string word, const int id);
		string GenerateConceptPrimaryKey(const shared_ptr<iConcept> concept);

		//////////////////////////////////////////////////////////////////////////
		//Check if mod string is a ConceptInteractTable.
		//////////////////////////////////////////////////////////////////////////
		bool IsModTable(const vector<string>& modSplit);

		//////////////////////////////////////////////////////////////////////////
		//Extract connection data from <row> and add to <concept>.
		//////////////////////////////////////////////////////////////////////////
		void AddConnectionToConcept(const CommonTool::DBRow& row, shared_ptr<Concept> concept);
		shared_ptr<iConceptInteractTable> ConvertModStringToConceptTable(const string modStr);

		//////////////////////////////////////////////////////////////////////////
		//Convert <row> to concept which contains connection data.
		//The returned concept is either a Non base concept or base concept.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<Concept> ConvertRowToConcept(const CommonTool::DBRow& row);

		//////////////////////////////////////////////////////////////////////////
		//Append a new segment to the tail of the raw string.
		//If the raw string is empty, then returned string equals the new segment.
		//Otherwise, append a blank between the raw string and the new segment.
		//////////////////////////////////////////////////////////////////////////
		void AppendSegmentToString(const string segment, string& raw);

		//////////////////////////////////////////////////////////////////////////
		//Change primary key of each row to a hash value computed from word and id.
		//////////////////////////////////////////////////////////////////////////
		void ChangePrimaryKeyToHash();

		//////////////////////////////////////////////////////////////////////////
		//Read concept connection from a file, and parse connections and add them to database.
		//////////////////////////////////////////////////////////////////////////
		void ReadConceptConnectionFromFile(const string filePath);
		//////////////////////////////////////////////////////////////////////////
		//Convert mod table string in the file to a single string in which concepts are represented by conceptID.
		//////////////////////////////////////////////////////////////////////////
		string ConvertModTableString(const string modTableStr);
		//////////////////////////////////////////////////////////////////////////
		//Convert single mod to a single string in which concepts are represented by conceptID.
		//Note!<singleModStr> is like '0','×Ô¼º','0','Äã'...
		//////////////////////////////////////////////////////////////////////////
		string ConvertSingleMod(const vector<string>& singleModStr,const string toConceptID);

		//////////////////////////////////////////////////////////////////////////
		//Clear connection column in the Non base concept table,
		//and then append connection from concept connection table to the connection column.
		//////////////////////////////////////////////////////////////////////////
		void RefreshConceptConnectionInConceptTable();
	};

	namespace NonBaseConceptField
	{
		static const string ConceptID = "conceptID";
		static const string ID = "id";
		static const string Word = "word";
		static const string POS = "pos";
		static const string Connection = "connection";
	};

	namespace BaseConceptField
	{
		static const string ConceptID = "conceptID";
		static const string ID = "id";
		static const string Word = "word";
		static const string BaseID = "baseID";
		static const string POS = "pos";
	};

	namespace ConceptConnectionField
	{
		static const string ConnectionID = "connectionID";
		static const string Concept = "concept";
		static const string ToConcept = "toConcept";
		static const string Modification = "modification";
	};
}


