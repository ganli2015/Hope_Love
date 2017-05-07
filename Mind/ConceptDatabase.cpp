#include "stdafx.h"
#include "ConceptDatabase.h"

#include "../MindElement/BaseConcept.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"

using namespace CommonTool;
using namespace DataCollection;

namespace Mind
{
	ConceptDatabase::ConceptDatabase():BaseConceptTable("BaseConceptsString")
	{
	}


	ConceptDatabase::~ConceptDatabase()
	{
	}

	void ConceptDatabase::AddBaseConcept(const shared_ptr<BaseConcept> concept)
	{
		long index = concept->GetBaseId();
		int id = concept->GetId();
		string word = concept->GetString();
		PartOfSpeech pos = concept->GetPartOfSpeech();

		AddBaseConcept(index, id, word, pos);
	}

	shared_ptr<BaseConcept> ConceptDatabase::ReadBaseConcept(const long index)
	{
		CheckConnect();
		//Get data from row.
		auto row = GetBaseConceptRow(index);
		long baseID= row.GetLong("baseID");
		int id = row.GetLong("id");
		string wordStr = row.GetText("word");
		PartOfSpeech pos = (PartOfSpeech)row.GetLong("pos");

		//Create the concept.
		shared_ptr<Word> word = LanguageFunc::GetParticularWord(wordStr, pos);
		shared_ptr<BaseConcept> concept = make_shared<BaseConcept>(word);
		concept->SetId(id);
		concept->SetBaseId(baseID);
		return concept;
	}

	void ConceptDatabase::AddBaseConcept(const long index, const int id, const string word, const DataCollection::PartOfSpeech pos)
	{
		CheckConnect();

		char state[100];
		sprintf_s(state, "Insert into %s (baseID,id,word,pos)\
				VALUES (:baseID,:id,:word,:pos) ", BaseConceptTable.c_str());

		DBCmd cmd(state, *_db);
		cmd.Bind(":baseID", index);
		cmd.Bind(":id", id);
		cmd.Bind(":word", CommonTool::AsciiToUtf8(word));
		cmd.Bind(":pos", (int)pos);

		cmd.Execute();
	}

	CommonTool::DBRow ConceptDatabase::GetBaseConceptRow(const long index)
	{
		CheckConnect();

		char state[100];
		sprintf_s(state, "Select * from %s where baseID='%ld'", BaseConceptTable.c_str(), index);

		DBQry qry(state, *_db);
		auto rows = qry.GetRows();
		if (rows.size() == 1)
		{
			//get unique value.
			return rows.front();
		}
		else
		{
			throw runtime_error("Invalid id: " + index);
		}
	}


}
