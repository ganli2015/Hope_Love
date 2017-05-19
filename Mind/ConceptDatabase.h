#pragma once
#include "MindDatabase.h"

namespace CommonTool
{
	class DBoperator;
	class DBRow;
}

namespace DataCollection
{
	enum PartOfSpeech;
	class Word;
}

namespace Mind
{
	class BaseConcept;

	//////////////////////////////////////////////////////////////////////////
	//Database for Base Concept, NonBase Concept and Concept Connection.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINOUT ConceptDatabase : public MindDatabase
	{
		const string BaseConceptTable;
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
		//<index> is the index in database.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<BaseConcept> ReadBaseConcept(const long index);
		
		//////////////////////////////////////////////////////////////////////////
		//Get total count of base concept.
		//////////////////////////////////////////////////////////////////////////
		size_t GetBaseConceptCount();

		//////////////////////////////////////////////////////////////////////////
		//Check if there is any concept with word <word>.
		//////////////////////////////////////////////////////////////////////////
		bool HasWord(const shared_ptr<DataCollection::Word> word);

	private:
		vector<CommonTool::DBRow> QueryRows(const string cmd);

		void AddBaseConcept(const long index, const int id, const string word, const DataCollection::PartOfSpeech pos);
		CommonTool::DBRow GetBaseConceptRow(const long index);

		vector<CommonTool::DBRow> GetRowsWithWord(const string word);
	};
}


