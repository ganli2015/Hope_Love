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
		//<index> is the index in database.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<BaseConcept> ReadBaseConcept(const long index);


	private:

		void AddBaseConcept(const long index, const int id, const string word, const DataCollection::PartOfSpeech pos);
		CommonTool::DBRow GetBaseConceptRow(const long index);
	};
}


