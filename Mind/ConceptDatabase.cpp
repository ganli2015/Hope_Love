#include "stdafx.h"
#include "ConceptDatabase.h"

#include "../MindElement/BaseConcept.h"
#include "../MindElement/MindElementCreator.h"

#include "../MindInterface/iMindElementCreator.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/QueryStatement.h"

using namespace CommonTool;
using namespace DataCollection;

namespace Mind
{
	ConceptDatabase::ConceptDatabase():BaseConceptTable("BaseConceptsString"),
		NonBaseConceptTable("NonBaseConcept")
	{
		_tables.push_back(BaseConceptTable);
		_tables.push_back(NonBaseConceptTable);
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

	void ConceptDatabase::AddBaseConcept(const shared_ptr<DataCollection::Word> word)
	{
		auto baseID = GetBaseConceptCount();
		//Get max ID for existed base concept with Word <word>.
		long maxID = -1;
		auto rows = GetRowsWithWord(word->GetString());
		for (auto row : rows)
		{
			auto id = row.GetLong("id");
			if (id > maxID)
			{
				maxID = id;
			}
		}

		AddBaseConcept(baseID, maxID + 1, word->GetString(), word->Type());
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
		shared_ptr<BaseConcept> concept = this->_elemCreator->CreateBaseConcept(word, id, baseID);
		return concept;
	}

	size_t ConceptDatabase::GetBaseConceptCount()
	{
		CheckConnect();
		char state[100];
		sprintf_s(state, "Select Count(*) from %s", BaseConceptTable.c_str());

		auto row = QueryRows(state).front();

		return row.GetLong("Count(*)");
	}

	bool ConceptDatabase::HasWord(const shared_ptr<DataCollection::Word> word)
	{
		CheckConnect();

		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("word", word->GetString());
			state.EQ("pos", word->Type());
		}

		auto rows = QueryForTables(statements);

		return !rows.empty();
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
		QueryStatement state (BaseConceptTable);
		state.EQ("baseID", index);

		auto rows = QueryRows(state);
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


	vector<DBRow> ConceptDatabase::GetRowsWithWord(const string word)
	{
		char state[100];
		sprintf_s(state, "Select * from %s where word='%s'",
			BaseConceptTable.c_str(), CommonTool::AsciiToUtf8(word).c_str());

		return QueryRows(state);
	}

	vector<CommonTool::DBRow> ConceptDatabase::QueryForTables(const vector<CommonTool::QueryStatement>& statements)
	{
		vector<CommonTool::DBRow> res;
		for (auto state : statements)
		{
			auto rows = QueryRows(state);
			res.insert(res.end(), rows.begin(), rows.end());
		}

		return res;
	}

	vector<CommonTool::QueryStatement> ConceptDatabase::CreateQryForTables() const
	{
		vector<CommonTool::QueryStatement> res;
		for (auto table : _tables)
		{
			res.push_back(QueryStatement(table));
		}
		return res;
	}

	vector<DBRow> ConceptDatabase::QueryRows(const CommonTool::QueryStatement& state)
	{
		return QueryRows(state.GetString());
	}

	vector<CommonTool::DBRow> ConceptDatabase::QueryRows(const string& cmd)
	{
		CheckConnect();

		DBQry qry(cmd, *_db);
		auto rows = qry.GetRows();
		return rows;

		//return vector<CommonTool::DBRow>();
	}

}
