#include "stdafx.h"
#include "ConceptDatabase.h"

#include "ConceptSet.h"
#include "ConceptSetInitializer.h"

#include "../MindElement/BaseConcept.h"
#include "../MindElement/MindElementCreator.h"

#include "../MindInterface/iMindElementCreator.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/QueryStatement.h"
#include "../CommonTools/UpdateStatement.h"

using namespace CommonTool;
using namespace DataCollection;

namespace Mind
{
	ConceptDatabase::ConceptDatabase():BaseConceptTable("BaseConceptsString"),
		NonBaseConceptTable("NonBaseConcept"),
		ConceptConnectionTable("ConceptConnection")
	{
		_tables.push_back(BaseConceptTable);
		_tables.push_back(NonBaseConceptTable);
	}


	ConceptDatabase::~ConceptDatabase()
	{
	}

	void ConceptDatabase::AddBaseConcept(const shared_ptr<BaseConcept> concept)
	{
		long baseId = concept->GetBaseId();
		int id = concept->GetId();
		string word = concept->GetString();
		PartOfSpeech pos = concept->GetPartOfSpeech();

		AddBaseConcept(baseId, id, word, pos);
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

	void ConceptDatabase::AddNonBaseConcept(const shared_ptr<DataCollection::Word> word)
	{
		//Compute the max ID of concepts with word <word>.
		//And the newly added concept'ID is the max ID increased by one.

		auto conceptWithWords = GetConceptsWithWord(word->GetString());
		int maxID = -1;//If there is no concepts, the new ID is automatically zero.
		for (auto concept : conceptWithWords)
		{
			auto id = concept->GetId();
			if (id > maxID)
			{
				maxID = id;
			}
		}

		AddNonBaseConcept(maxID + 1, word->GetString(), word->Type());
	}

	void ConceptDatabase::AddNonBaseConcept(const shared_ptr<DataCollection::Word> word, const int id)
	{
		AddNonBaseConcept(id, word->GetString(), word->Type());
	}

	shared_ptr<BaseConcept> ConceptDatabase::GetBaseConcept(const long baseID)
	{
		CheckConnect();
		//Get data from row.
		auto row = GetBaseConceptRow(baseID);

		return _elemCreator->CreateBaseConcept(row);
	}

	shared_ptr<BaseConcept> ConceptDatabase::GetBaseConcept(const int id, const string word)
	{
		QueryStatement state(BaseConceptTable);
		state.EQ("id", id);
		state.EQ("word", word);

		auto rows = QueryRows(state);
		if (rows.size() == 1)
		{
			//get unique value.
			return _elemCreator->CreateBaseConcept(rows.front()); 
		}
		else
		{
			throw runtime_error("Invalid word: " + word);
		}
	}

	shared_ptr<Concept> ConceptDatabase::GetNonBaseConcept(const int id, const string word)
	{
		CheckConnect();

		auto row = GetNonBaseConceptRow(id, word);

		return ConvertRowToConcept(row);
	}

	shared_ptr<Concept> ConceptDatabase::GetConcept(const shared_ptr<DataCollection::Word> word)
	{
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("word", word->GetString());
			state.EQ("pos", word->Type());
		}

		auto rows = QueryForTables(statements);
		if (rows.empty())
			return NULL;
		else
		{
			return ConvertRowToConcept(rows.front());
		}
	}

	shared_ptr<Concept> ConceptDatabase::GetConcept(const string word, const int id)
	{
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("word", word);
			state.EQ("id", id);
		}

		auto rows = QueryForTables(statements);
		if (rows.empty())
			return NULL;
		else
		{
			return ConvertRowToConcept(rows.front());
		}
	}

	shared_ptr<Concept> ConceptDatabase::GetConcept(const string conceptID)
	{
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("conceptID", conceptID);
		}

		auto rows = QueryForTables(statements);
		if (rows.empty())
			return NULL;
		else
		{
			return ConvertRowToConcept(rows.front());
		}
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
		auto concept = GetConcept(word);

		return concept != NULL;
	}

	bool ConceptDatabase::HasString(const string wordStr)
	{
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("word", wordStr);
		}

		auto rows = QueryForTables(statements);

		return !rows.empty();
	}

	vector<shared_ptr<Concept>> ConceptDatabase::GetConceptsWithHead(const shared_ptr<DataCollection::Character> headChara)
	{
		string startChara = headChara->GetString();
		//Query rows.
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.Like("word", StringFormat("%s%%",startChara.c_str()));
		}
		auto rows = QueryForTables(statements);

		//Transform rows to concepts.
		vector<shared_ptr<Concept>> res;
		for (auto row : rows)
		{
			auto concept = ConvertRowToConcept(row);
			res.push_back(concept);
		}

		return res;
	}

	vector<shared_ptr<Concept>> ConceptDatabase::GetConceptsWithWord(const string word)
	{
		//Query rows.
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("word", word);
		}
		auto rows = QueryForTables(statements);

		//Transform rows to concepts.
		vector<shared_ptr<Concept>> res;
		for (auto row : rows)
		{
			auto concept = ConvertRowToConcept(row);
			res.push_back(concept);
		}

		return res;
	}

	vector<shared_ptr<Concept>> ConceptDatabase::GetConceptsWithPOS(const DataCollection::PartOfSpeech pos)
	{
		auto statements = CreateQryForTables();
		for (int i = 0; i < statements.size(); ++i)
		{
			auto &state = statements[i];

			state.EQ("pos", pos);
		}
		auto rows = QueryForTables(statements);

		//Transform rows to concepts.
		vector<shared_ptr<Concept>> res;
		for (auto row : rows)
		{
			auto concept = ConvertRowToConcept(row);
			res.push_back(concept);
		}

		return res;
	}

	void ConceptDatabase::AddBaseConcept(const long baseID, const int id, const string word, const DataCollection::PartOfSpeech pos)
	{
		CheckConnect();

		char state[1024];
		sprintf_s(state, "Insert into %s (conceptID,baseID,id,word,pos)\
				VALUES (:conceptID,:baseID,:id,:word,:pos) ", BaseConceptTable.c_str());

		string pk = GenerateConceptPrimaryKey(word, id);

		DBCmd cmd(state, *_db);
		cmd.Bind(":conceptID", pk);
		cmd.Bind(":baseID", baseID);
		cmd.Bind(":id", id);
		cmd.Bind(":word", CommonTool::AsciiToUtf8(word));
		cmd.Bind(":pos", (int)pos);
		
		cmd.Execute();
	}

	void ConceptDatabase::AddNonBaseConcept(const int id, const string word, const DataCollection::PartOfSpeech pos)
	{
		CheckConnect();

		string state = StringFormat("Insert into %s(conceptID,id, word, pos)\
			VALUES(:conceptID,:id, :word, :pos) ", NonBaseConceptTable.c_str());

		string pk = GenerateConceptPrimaryKey(word, id);

		DBCmd cmd(state, *_db);
		cmd.Bind(":conceptID", pk);
		cmd.Bind(":id", id);
		cmd.Bind(":word", CommonTool::AsciiToUtf8(word));
		cmd.Bind(":pos", (int)pos);

		cmd.Execute();
	}

	CommonTool::DBRow ConceptDatabase::GetBaseConceptRow(const long baseID)
	{
		QueryStatement state (BaseConceptTable);
		state.EQ("baseID", baseID);

		auto rows = QueryRows(state);
		if (rows.size() == 1)
		{
			//get unique value.
			return rows.front();
		}
		else
		{
			throw runtime_error("Invalid id: " + baseID);
		}
	}


	CommonTool::DBRow ConceptDatabase::GetNonBaseConceptRow(const int id, const string word)
	{
		QueryStatement state(NonBaseConceptTable);
		state.EQ("id", id);
		state.EQ("word", word);

		auto rows = QueryRows(state);
		if (rows.size() == 1)
		{
			//get unique value.
			return rows.front();
		}
		else
		{
			throw runtime_error("Invalid word: " + word);
		}
	}

	CommonTool::DBRow ConceptDatabase::GetRow(const string pk, const string pkColName, const string table)
	{
		QueryStatement qryState(table);
		qryState.EQ(pkColName, pk);
		DBQry qry(qryState.GetString(), *_db);
		auto rows = qry.GetRows();
		//There must be only one row.
		return rows.front();
	}

	vector<DBRow> ConceptDatabase::GetRowsWithWord(const string word)
	{
		auto statements = CreateQryForTables();
		for (auto &state : statements)
		{
			state.EQ("word", word);
		}

		return QueryForTables(statements);
	}

	CommonTool::DBRow ConceptDatabase::GetRowInConceptTables(const string pk)
	{
		auto statements = CreateQryForTables();
		for (auto &state : statements)
		{
			state.EQ(NonBaseConceptField::ConceptID, pk);
		}

		return QueryForTables(statements).front();
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

	vector<shared_ptr<BaseConcept>> ConceptDatabase::GetAllBaseConcepts()
	{
		QueryStatement qry(BaseConceptTable);
		auto allrows = QueryRows(qry);

		vector<shared_ptr<BaseConcept>> res;
		for (auto row : allrows)
		{
			res.push_back(_elemCreator->CreateBaseConcept(row));
		}

		return res;
	}

	vector<shared_ptr<Concept>> ConceptDatabase::GetAllNonBaseConcepts()
	{
		QueryStatement qry(NonBaseConceptTable);
		auto allrows = QueryRows(qry);

		vector<shared_ptr<Concept>> res;
		for (auto row : allrows)
		{
			res.push_back(ConvertRowToConcept(row));
		}

		return res;
	}

	std::string ConceptDatabase::GenerateConceptPrimaryKey(const string word, const int id)
	{
		string joinStr = StringFormat("%d-%s", id, word.c_str());
		//Compute hash.
		string newID = GenerateHash(joinStr);

		return newID;
	}

	bool ConceptDatabase::IsModTable(const vector<string>& modSplit)
	{
		if (modSplit.size() > 1)
		{
			return false;
		}
		else
		{
			//Mod table only contains one single string.
			return true;
		}
	}

	void ConceptDatabase::AddConnectionToConcept(const CommonTool::DBRow& row, shared_ptr<Concept> concept)
	{
		if (!row.HasColumn(NonBaseConceptField::Connection)) return;

		auto connectionStr = row.GetText(NonBaseConceptField::Connection);
		auto connectionIDs = SplitString(connectionStr, ' ');//There may be many connections.
		for (auto id : connectionIDs)
		{
			//There must be only one row.
			auto row = GetRow(id, ConceptConnectionField::ConnectionID,ConceptConnectionTable);
			//Get 'to concept'.
			auto to_conceptID = row.GetText(ConceptConnectionField::ToConcept);
			auto toConceptRow = GetRowInConceptTables(to_conceptID);
			auto to_ID = toConceptRow.GetLong(NonBaseConceptField::ID);
			auto to_Word = toConceptRow.GetText(NonBaseConceptField::Word);
			//Recursively get toConcept as it may depend on other concepts.
			//Recursion will stop when the current concept is a base concept which depends nothing.
			auto to_Concept = GetConcept(to_Word, to_ID);

			concept->AddForwardConcept(to_Concept);

			//Add modification.
			auto modStr = row.GetText(ConceptConnectionField::Modification);
			if (modStr != "")
			{
				auto modTable = ConvertModStringToConceptTable(modStr);
				concept->AddForwardModification(to_Concept, modTable);
			}
		}
		
	}

	shared_ptr<iConceptInteractTable> ConceptDatabase::ConvertModStringToConceptTable(const string modStr)
	{
		vector<pair<shared_ptr<iConcept>, shared_ptr<iConcept>>> res;

		auto modStrSplit = SplitString(modStr, ' ');
		for (auto modPair : modStrSplit)
		{
			//modPair is like 'xxxx-yyyyy'.
			auto conceptSplit = SplitString(modPair, '-');
			assert(conceptSplit.size() == 2);

			//Find concept by conceptID.
			shared_ptr<iConcept> fromConcept = GetConcept(conceptSplit[0]);
			shared_ptr<iConcept> toConcept = GetConcept(conceptSplit[1]);

			res.push_back(make_pair(fromConcept, toConcept));
		}

		return _elemCreator->CreateConceptInteractTable(res);
	}

	shared_ptr<Concept> ConceptDatabase::ConvertRowToConcept(const CommonTool::DBRow& row)
	{
		//Get a concept with no connection.
		auto concept = _elemCreator->CreateConcept(row);
		//Append connection.
		AddConnectionToConcept(row, concept);

		return concept;
	}

	void ConceptDatabase::ChangePrimaryKeyToHash()
	{
		//Get all base concepts.
		auto baseConcepts = GetAllBaseConcepts();
		//Delete all rows.
		_db->DeleteRowsInTable(BaseConceptTable);

		_db->BeginTransaction();

		for (auto base : baseConcepts)
		{
			//When re-insert the base concept, the hash value will be automatically computed.
			AddBaseConcept(base);
		}

		_db->CommitTransaction();

		//Non base concepts.

		auto nonBaseConcepts = GetAllNonBaseConcepts();
		//Delete all rows.
		_db->DeleteRowsInTable(NonBaseConceptTable);

		_db->BeginTransaction();

		for (auto concept : nonBaseConcepts)
		{
			//When re-insert the concept, the hash value will be automatically computed.
			AddNonBaseConcept(concept->GetWord(),concept->GetId());
		}
		_db->CommitTransaction();
	}

	void ConceptDatabase::ReadConceptConnectionFromFile(const string filePath)
	{
		//Clear table "ConceptConnection".
		_db->DeleteRowsInTable(ConceptConnectionTable);

		//Read connections from file.
		ifstream in(filePath);
		vector<Connection_Info> connections;
		string str;
		while (getline(in, str))
		{
			Connection_Info connnection_info = ConceptSetInitializer::ParseStrToSimpleConnectionInfo(str);

			connections.push_back(connnection_info);
		}

		_db->BeginTransaction();

		for (auto connection : connections)
		{
			//Current concept.
			auto me_ID = GenerateConceptPrimaryKey(connection.me.str, connection.me.id);
			//Handle edges.
			for (auto edge: connection.edge_infos)
			{
				auto to_ID = GenerateConceptPrimaryKey(edge.to.str, edge.to.id);
				
				string conncetionID = GenerateHash(me_ID + to_ID);

				//Handle modification string.
				string modStr = "";
				if (edge.modStr != "")
				{
					auto modSplit = SplitString(edge.modStr, ' ');
					if (IsModTable(modSplit))
					{
						modStr = ConvertModTableString(modSplit.front());
					}
					else
					{
						modStr = ConvertSingleMod(modSplit, to_ID);
					}
				}

				//Insert to database.
				string insertCmd = StringFormat("Insert into %s(connectionID,concept, toConcept,modification)\
					VALUES(:connectionID,:concept, :toConcept,:modification) ", ConceptConnectionTable.c_str());

				DBCmd cmd(insertCmd, *_db);
				cmd.Bind(":connectionID", conncetionID);
				cmd.Bind(":concept", me_ID);
				cmd.Bind(":toConcept", to_ID);
				cmd.Bind(":modification", AsciiToUtf8(modStr));

				cmd.Execute();
			}
		}

		_db->CommitTransaction();
	}

	std::string ConceptDatabase::ConvertModTableString(const string modTableStr)
	{
		string res = "";

		auto modTableSplit = SplitString(modTableStr, ',');
		for (int i = 0; i < modTableSplit.size(); ++i)
		{
			auto pair = modTableSplit[i];

			auto pairSplit = SplitString(pair, '-');
			assert(pairSplit.size() == 2);

			auto conceptFrom = pairSplit[0];
			auto conceptTo = pairSplit[1];
			//From concept.
			auto conceptFromSplit = SplitString(conceptFrom, '@');
			auto fromWord = conceptFromSplit[1];
			auto fromID = CommonTool::StrToInt(conceptFromSplit[0]);
			string fromConceptID = GenerateConceptPrimaryKey(fromWord, fromID);
			//To concept.
			auto conceptToSplit = SplitString(conceptTo, '@');
			auto toWord = conceptToSplit[1];
			auto toID = CommonTool::StrToInt(conceptToSplit[0]);
			string toConceptID = GenerateConceptPrimaryKey(toWord, toID);

			res += fromConceptID + "-" + toConceptID;
			if (i != modTableSplit.size() - 1)
			{
				res += " ";
			}
		}

		return res;
	}

	std::string ConceptDatabase::ConvertSingleMod(const vector<string>& singleModStr, const string toConceptID)
	{
		string res = "";

		for (int i = 0; i < singleModStr.size(); i += 2)
		{
			auto id = CommonTool::StrToInt(singleModStr[i]);
			auto word= singleModStr[i+1];

			string conceptID = GenerateConceptPrimaryKey(word, id);
			res += conceptID + '-' + toConceptID;
			if (i != singleModStr.size() - 2)
			{
				res += " ";
			}
		}

		return res;
	}

	void ConceptDatabase::RefreshConceptConnectionInConceptTable()
	{
		//Get all concept rows.
		QueryStatement qryState(NonBaseConceptTable);
		DBQry qry(qryState.GetString(), *_db);
		auto conceptRows = qry.GetRows();

		for (auto concept : conceptRows)
		{
			auto conceptID = concept.GetText("conceptID");
			//Check if <concept> exists in concept connection table.
			QueryStatement findConceptQry(ConceptConnectionTable);
			findConceptQry.EQ("concept", conceptID);

			DBQry qry2(findConceptQry.GetString(), *_db);
			auto connectionRows = qry2.GetRows();
			//Join connection ID to a single string.
			string idStr = "";
			for (size_t i = 0; i < connectionRows.size(); ++i)
			{
				idStr += connectionRows[i].GetText("connectionID");
				if (i != connectionRows.size() - 1)
				{
					idStr += " ";
				}
			}
			//Update connection column in concept table.
			UpdateStatement updateQry(NonBaseConceptTable);
			updateQry.Update("connection", idStr);
			updateQry.EQ("conceptID", conceptID);
			DBCmd cmd(updateQry.GetString(), *_db);
			cmd.Execute();
		}
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
	}

}
