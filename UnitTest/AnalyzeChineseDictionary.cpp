#include "stdafx.h"
#include "AnalyzeChineseDictionary.h"
#include "FuncForTest.h"
#include "TestPOSTagging.h"
#include "ChinesePOSDictionary.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/Character.h"
#include "../DataCollection/LanguageFunc.h"

#include "../Mind/Cerebrum.h"

#include "../MindInterface/iConcept.h"

#include "../Mathmatic/DirectedGraph.h"
#include "../Mathmatic/TraverseData.h"

#include "../MindDatabase/ConceptDatabase.h"

#include "../MindElement/Concept.h"
#include "../MindElement/BaseConcept.h"

using namespace Mind;
using namespace CommonTool;
using namespace DataCollection;
using namespace Math;

AnalyzeChineseDictionary::AnalyzeChineseDictionary()
{
	Mind::SetHopeLoveMindPath(FuncForTest::LargeDataPath);
	iCerebrum::SetInstance(Cerebrum::Instance());
}


AnalyzeChineseDictionary::~AnalyzeChineseDictionary()
{
}

void AnalyzeChineseDictionary::ExtractBaseWords(const string filePath)
{
	ExtractAllDefinitions(filePath);

	DistinguishBaseConceptAndNonBase();

	OutputBaseWords(BASE_WORD_FILE,_baseWordDefs);
	OutputBaseWords(NON_BASE_WORD_FILE, _nonBaseWordDefs);
}

void AnalyzeChineseDictionary::BuildConnection(const string filePath)
{
	ExtractAllDefinitions(filePath);

	ReadWordsFromFile(BASE_WORD_FILE, _baseWordDefs);

	SegmentMeanings();

	FindConnection();

	OutputWordConnection();
}

void AnalyzeChineseDictionary::AnalyzeValidConnections(const string filePath)
{
	auto wordConnections = ReadWordConnections(filePath);

	auto wordGraph = BuildGraph(wordConnections);

	auto subgraphs = wordGraph->AllSubGraphs();
	cout << subgraphs.size() << endl;

	auto validConnections = GetValidConnections(subgraphs);

	OutputValidConnections(validConnections);
}

void AnalyzeChineseDictionary::OutputToDB(const string connectionPath, const string validConnectionPath)
{
	auto wordConnections = ReadWordConnections(connectionPath);

	auto validWords = ReadValidConnectionWords("valid_connection.txt");

	RemoveInvalidWords(validWords, wordConnections);

	ReadWordsFromFile(BASE_WORD_FILE, _baseWordDefs);

	RefactorConceptInDatabase(wordConnections);

	OutputToConceptConnectionToDatabase(wordConnections);
}

void AnalyzeChineseDictionary::ExtractAllDefinitions(const string filePath)
{
	ifstream in(filePath);
	string line = "";

	//Extract word and meanings from the file.
	while (getline(in, line))
	{
		if (line == "") continue;

		shared_ptr<WordDefinition> wordDef;
		if (ExtractDefinition(line, wordDef))
		{
			_allWordDefs[wordDef->GetWord()] = (wordDef);
		}
	}
}

bool AnalyzeChineseDictionary::ExtractDefinition(const string line, shared_ptr<WordDefinition>& def) const
{
	if (CommonTool::FindChar(line, '*')!=line.cend()) return false;

	//Extract word.The word is in the brackets "¡¾...¡¿".
	string lbra = "¡¾";
	string rbra = "¡¿";
	auto leftBra = CommonTool::FindString(line, lbra);
	auto rightBra = CommonTool::FindString(line, rbra);
	string word = string(leftBra + lbra.length(), rightBra);

	def = make_shared<WordDefinition>(word);

	//Extract meanings.
	string indexOne = "£¨£±£©";
	string indexTwo = "£¨£²£©";
	string indexThree = "£¨£³£©";
	if (CommonTool::FindString(line, indexOne) == line.end())
	{
		//There is only one meaning.
		string meaning(rightBra + rbra.length(), line.end());
		def->AddMeaning(ExtractMeaning(meaning));
	}
	else
	{
		//Extract first Three meanings.
		if(CommonTool::HasString(line, indexOne))
			def->AddMeaning(FindMeaning(line, indexOne));
		if (CommonTool::HasString(line, indexTwo))
			def->AddMeaning(FindMeaning(line, indexTwo));
		if (CommonTool::HasString(line, indexThree))
			def->AddMeaning(FindMeaning(line, indexThree));
	}

	return true;
}

std::string AnalyzeChineseDictionary::FindMeaning(const string line, const string indexTag) const
{
	//Select string between two indices brackets.
	auto indexLoc = CommonTool::FindString(line, indexTag);
	auto nextBraLoc= CommonTool::FindString(line, indexLoc + indexTag.length(),"£¨");

	string wholeMeaning= string(indexLoc + indexTag.length(), nextBraLoc);
	return ExtractMeaning(wholeMeaning);
}

void AnalyzeChineseDictionary::DistinguishBaseConceptAndNonBase()
{
	iCerebrum *brain = iCerebrum::Instance();
	auto allBaseConcepts = brain->GetAllBaseConcepts();

	//Generate a map with single character string as key and word definition as value.
	//The map makes convenience for querying concept frequency.
	multimap<string, shared_ptr<WordDefinition>> char_wordDefs = GenerateCharacterWordDefMap();

	//Find frequency of words in iCerebrum.
	vector<pair<string, long>> word_freq;
	set<string> addedWord;
	for (auto concept : allBaseConcepts)
	{
		string word = concept->GetWord()->GetString();
		if (addedWord.find(word) != addedWord.end())
		{
			continue;
		}

		long freq = ComputeFreq(word,char_wordDefs);

		if (freq != 0)
		{
			word_freq.push_back(make_pair(word, freq));
			addedWord.insert(word);
		}
	}

	//Count words with most frequency.
	sort(word_freq.begin(), word_freq.end(), [=](std::pair<string, long>& a, std::pair<string, long>& b)
	{
		return a.second > b.second;
	}
	);

	//Sort word definitions by frequency according to <word_freq>.
	vector<pair<string,shared_ptr<WordDefinition>>> sortedByFreqWordDefs;
	for (auto word_freq_pair :word_freq)//<word_freq> is sorted by frequency.
	{
		string word = word_freq_pair.first;
		if (_allWordDefs.count(word))
		{
			auto wordDef = _allWordDefs[word];
			wordDef->Freq(word_freq_pair.second);
			sortedByFreqWordDefs.push_back(make_pair(word,wordDef));
		}
	}

	_baseWordDefs.insert(sortedByFreqWordDefs.begin(), sortedByFreqWordDefs.begin() + BASE_COUNT);
	_nonBaseWordDefs.insert(sortedByFreqWordDefs.begin()+ BASE_COUNT, sortedByFreqWordDefs.end());


	//Count frequency of word with single character.
	vector<pair<string, long>> singleChar_freq;
	for (auto concept : allBaseConcepts)
	{
		auto word = concept->GetWord();
		if(word->Type()==DataCollection::Punctuation) continue;
		if (word->NumOfChara() == 1)
		{
			long freq = ComputeFreq(word->GetString(), char_wordDefs);
			singleChar_freq.push_back(make_pair(word->GetString(),freq));
		}
	}

	//Sort <singleChar_freq>.
	sort(singleChar_freq.begin(), singleChar_freq.end(), [=](std::pair<string, long>& a, std::pair<string, long>& b)
	{
		return a.second > b.second;
	}
	);

	//Append to <_baseWordDefs>.
	for (int i=0;i<BASE_COUNT_SINGLE_CHAR;++i)
	{
		shared_ptr<WordDefinition> wordDef = make_shared<WordDefinition>(singleChar_freq[i].first);
		wordDef->Freq(singleChar_freq[i].second);
		_baseWordDefs[singleChar_freq[i].first] = wordDef;
	}
}

multimap<string, shared_ptr<WordDefinition>> AnalyzeChineseDictionary::GenerateCharacterWordDefMap() const
{
	multimap<string, shared_ptr<WordDefinition>> char_wordDefs;
	for (auto wordDef : _allWordDefs)
	{
		try
		{
			auto chars = GenerateCharacter(wordDef.second);
			for (auto ch : chars)
			{
				char_wordDefs.insert(make_pair(ch, wordDef.second));
			}
		}
		catch (const std::exception& ex)
		{
			continue;
		}
	}

	return char_wordDefs;
}

set<string> AnalyzeChineseDictionary::GenerateCharacter(const shared_ptr<WordDefinition> line) const
{
	set<string> res;
	for (auto meaning : line->GetMeanings())
	{
		auto chars = LanguageFunc::ConvertStringToCharacter(meaning);
		for (auto ch : chars)
		{
			res.insert(ch->GetString());
		}
	}

	return res;
}

long AnalyzeChineseDictionary::ComputeFreq(const string word, multimap<string, shared_ptr<WordDefinition>>& wordDefMap) const
{
	auto chars = LanguageFunc::ConvertStringToCharacter(word);
	//WordDefinition that contains one of <chars>.
	set<shared_ptr<WordDefinition>> relatedWordDefs;
	for (auto ch : chars)
	{
		if (wordDefMap.find(ch->GetString()) == wordDefMap.end()) continue;

		//Find all word definition containing <ch>.
		auto allDefs = FindAll(ch->GetString(), wordDefMap);

		relatedWordDefs.insert(allDefs.begin(), allDefs.end());
	}

	long freq = 0;
	for (auto wordDef : relatedWordDefs)
	{
		if (MeaningHasWord(wordDef, word))
		{
			freq++;
		}
	}

	return freq;
}

bool AnalyzeChineseDictionary::MeaningHasWord(const shared_ptr<WordDefinition>& wordDef, const string word) const
{
	for (auto meaning : wordDef->GetMeanings())
	{
		if (CommonTool::HasString(meaning, word))
		{
			return true;
		}
	}

	return false;
}

std::string AnalyzeChineseDictionary::ExtractMeaning(const string& line) const
{
	auto beforeColon = CommonTool::FindString(line, "£º");
	return string(line.begin(), beforeColon);
}

void AnalyzeChineseDictionary::OutputBaseWords(const string filePath,const map<string, shared_ptr<WordDefinition>>& wordMap) const
{
	ofstream out(filePath);

	for (auto baseDef : wordMap)
	{
		out << baseDef.first << "  " << baseDef.second->Freq() << endl;
	}

	out.flush();
	out.close();
}

void AnalyzeChineseDictionary::ReadWordsFromFile(const string filePath, map<string, shared_ptr<WordDefinition>>& wordMap) const
{
	ifstream in(filePath);
	string line = "";
	while (getline(in, line))
	{
		if (line == "") continue;

		auto split = SplitString(line, ' ');
		CommonTool::RemoveEmptyString(split);
		Check(split.size() == 2);

		auto word = split.front();

		//Append word to wordMap.
		//Find word in <_allWordDefs>.
		if (_allWordDefs.find(word) != _allWordDefs.end())
		{
			auto wordDef = _allWordDefs.at(word);
			wordMap[word] = wordDef;
		}
		else
		{
			//Create a new one.
			auto newWordDef = make_shared<WordDefinition>(word);
			wordMap[word] = newWordDef;
		}
	}
}

void AnalyzeChineseDictionary::SegmentMeanings()
{
	int count = 0;

	TestPOSTagging posTagging;
	for (auto wordDef : _allWordDefs)
	{
		LOG_FORMAT("Word Definition: %s", wordDef.second->GetWord().c_str());
		for (auto meaning : wordDef.second->GetMeanings())
		{
			if (meaning == "") continue;
			//Filter long meaning.
			if(meaning.size()>30) continue;

			try
			{
				LOG_FORMAT("Begin segment: %s", meaning.c_str());
				auto segment = posTagging.SegmentWord(meaning);

				string segStr = "";
				for (auto seg : segment)
				{
					segStr += seg + " ";
				}
				LOG(segStr);

				wordDef.second->SetMeaningSegment(meaning, segment);
			}
			catch (const std::exception&)
			{
				ERROR_FORMAT("Fail to segment meaning: %s.", meaning.c_str());
			}

		}

		++count;
		if (count % 1000 == 0)
		{
			LOG_FORMAT("Have segmented: %d", count);
		}
	}
}

void AnalyzeChineseDictionary::FindConnection()
{
	//Generate a map in which key is single Character and value is related word definition.
	//And word definitions come from <_allWordDefs>.
	auto char_wordDef_map = GenerateCharacterWordDefMap();

	for (auto pair : _allWordDefs)
	{
		string word = pair.first;
		auto wordDef = pair.second;

		try
		{
			FindWhoDependOnMe(word, wordDef, char_wordDef_map);
		}
		catch (const std::exception&)
		{
			ERROR_FORMAT("Fail to handle word: %s.", word.c_str());
		}
	}

	for (auto pair : _baseWordDefs)
	{
		string word = pair.first;
		auto wordDef = pair.second;

		try
		{
			FindWhoDependOnMe(word, wordDef, char_wordDef_map);
		}
		catch (const std::exception&)
		{
			ERROR_FORMAT("Fail to handle word: %s.", word.c_str());
		}
	}
}

void AnalyzeChineseDictionary::FindWhoDependOnMe(const string word, 
	const shared_ptr<WordDefinition> wordDef,
	const multimap<string, shared_ptr<WordDefinition>>& wordDefMap)
{
	auto chars = LanguageFunc::ConvertStringToCharacter(word);
	//WordDefinition that contains one of <chars>.
	set<shared_ptr<WordDefinition>> relatedWordDefs;
	for (auto ch : chars)
	{
		if (wordDefMap.find(ch->GetString()) == wordDefMap.end()) continue;

		//Find all word definition containing <ch>.
		auto allDefs = FindAll(ch->GetString(), wordDefMap);

		relatedWordDefs.insert(allDefs.begin(), allDefs.end());
	}

	//Find if <relatedWordDefs> contains <word>
	for (auto related : relatedWordDefs)
	{
		if (related->HasWordInMeaning(word))
		{
			//TODO : Check for each meaning respectively.
			//Different meanings correspond to Different concepts.
			related->AddDependence(wordDef);
		}
	}
}

void AnalyzeChineseDictionary::OutputWordConnection() const
{
	ofstream out(WORD_CONNECTION_FILE);

	for (auto pair : _allWordDefs)
	{
		pair.second->Write(out);
	}

	out.flush();
	out.close();
}

map<string, shared_ptr<WordConnection>> AnalyzeChineseDictionary::ReadWordConnections(const string filePath) const
{
	map<string, shared_ptr<WordConnection>> res;

	//Read non base words. 
	ifstream in(filePath);
	string line = "";
	while (getline(in, line))
	{
		auto split = CommonTool::SplitString(line, ' ');
		CommonTool::RemoveEmptyString(split);

		if(split.size()<=1) continue;

		//Build connection of one word.
		shared_ptr<WordConnection> wordConnection=make_shared<WordConnection>(split.front());
		for (size_t i = 1; i < split.size(); ++i)
		{
			wordConnection->AddConnection(split[i]);
		}

		res[split.front()] = wordConnection;
	}

	//Read base words.
	map<string, shared_ptr<WordDefinition>> baseWords;
	ReadWordsFromFile(BASE_WORD_FILE, baseWords);

	for (auto basePair : baseWords)
	{
		shared_ptr<WordConnection> wordConnection = make_shared<WordConnection>(basePair.first);
		wordConnection->IsBase();
		res[basePair.first] = wordConnection;
	}

	return res;
}

shared_ptr<Math::DirectedGraph> AnalyzeChineseDictionary::BuildGraph(const map<string, shared_ptr<WordConnection>> wordConnections) const
{
	shared_ptr<Math::DirectedGraph> wordGraph=make_shared<DirectedGraph>(wordConnections.size());
	
	for (auto connection : wordConnections)
	{
		auto fromWord = connection.second;
		//Get to_ID
		for (auto to : connection.second->GetConnections())
		{
			if (wordConnections.find(to) == wordConnections.end())
			{
				//Cannot find such word.
				WARN_FORMAT("Cannot find word '%s' in wordConnections.", to);
				continue;
			}

			auto toWord = wordConnections.at(to);
			wordGraph->AddEdge(fromWord, toWord);
		}
	}

	return wordGraph;
}

vector<shared_ptr<WordConnection>> AnalyzeChineseDictionary::GetValidConnections(const vector<shared_ptr<DirectedGraph>>& graphs)
{
	vector<shared_ptr<WordConnection>> res;

	for (auto graph : graphs)
	{
		//Check if it has cycle.
		if (graph->HasCycle())
		{
			continue;
		}

		//Check if it contains at least one base word.
		bool hasBaseWord = false;
		auto allVerts = graph->GetAllVertices();
		for (auto vert : allVerts)
		{
			auto wordConnection = dynamic_pointer_cast<WordConnection>(vert);
			if (wordConnection->IsBaseWord())
			{
				hasBaseWord = true;
				break;
			}
		}
		if (!hasBaseWord)
		{
			continue;
		}

		//Check each vertex reaches a base word.
		for (auto vert : allVerts)
		{
			auto wordConnection = dynamic_pointer_cast<WordConnection>(vert);

			auto traverseData = graph->BFS(vert);
			auto traverseVertice = traverseData.GetAllConnectVertice();
			//Find if there is a base word in BFS of vert.
			auto findBase = find_if(traverseVertice.begin(), traverseVertice.end(),
				[](const shared_ptr<IVertex> tmpVert) -> bool{
				auto tmpConnection = dynamic_pointer_cast<WordConnection>(tmpVert);
				return tmpConnection->IsBaseWord();
			});

			if (findBase != traverseVertice.end())
			{
				DEBUG_FORMAT("Push back graph of word: %s", wordConnection->GetWord().c_str());
				//Has base.
				res.push_back(wordConnection);
			}
		}
	}

	return res;
}

void AnalyzeChineseDictionary::OutputValidConnections(const vector<shared_ptr<WordConnection>>& wordConnections) const
{
	ofstream out(VALID_CONNECTION_FILE);
	for (auto connection : wordConnections)
	{
		out << connection->GetWord() << endl;
	}

	out.close();
}

set<string> AnalyzeChineseDictionary::ReadValidConnectionWords(const string filePath) const
{
	set<string> res;

	ifstream in(filePath);
	string line = "";
	while (getline(in, line))
	{
		//One line is one word.
		res.insert(line);
	}

	return res;
}

void AnalyzeChineseDictionary::RemoveInvalidWords(const set<string>& validWords, 
	map<string, shared_ptr<WordConnection>>& wordConnections) const
{
	vector<string> wordToRemove;

	for (map<string, shared_ptr<WordConnection>>::iterator it = wordConnections.begin();
		it != wordConnections.end(); ++it)
	{
		if (validWords.find(it->first) == validWords.end())
		{
			//The word is not valid,then remove.
			wordToRemove.push_back(it->first);
		}
		else
		{
			//Remove invalid connection in current WordConnection.
			it->second->RemoveInvalidConnection(validWords);
		}
	}

	//Remove keys
	for (auto toRemove : wordToRemove)
	{
		wordConnections.erase(toRemove);
	}
}

void AnalyzeChineseDictionary::RefactorConceptInDatabase(const map<string, shared_ptr<WordConnection>>& wordConnections) const
{
	ConceptDatabase *db = new ConceptDatabase(GetDatabasePath());
	db->Connect();
	db->BeginTransaction();
	//Move some base concepts to non base concept table.
	for (auto connectionPair : wordConnections)
	{
		auto wordConnection = connectionPair.second;
		if (!wordConnection->IsBaseWord())
		{
			//Remove concept in base concept table if exists and
			//add to non base concept table.
			auto conceptInBaseDb = db->GetBaseConcept(0, wordConnection->GetWord());

			if (conceptInBaseDb != NULL)
			{
				db->DeleteBaseConcept(wordConnection->GetWord(), 0);
				db->AddNonBaseConcept(conceptInBaseDb);
			}
		}
	}

	//Check base concepts match with <_baseWordDefs>, otherwise concepts will be removed from database.
	auto allBaseConcepts = db->GetAllBaseConcepts();
	for (auto baseConcept : allBaseConcepts)
	{
		if (_baseWordDefs.find(baseConcept->GetWord()->GetString()) == _baseWordDefs.end())
		{
			//Cannot find word of base concept in the <_baseWordDefs>.
			//Remove it from database.
			db->DeleteBaseConcept(baseConcept->GetWord()->GetString(), baseConcept->GetId());
		}
	}

	//Consider words with connections as non base concept,
	//and append them to non base concept table.
	for (auto connectionPair : wordConnections)
	{
		auto connection = connectionPair.second;
		if (!connection->GetConnections().empty())
		{
			if (db->HasString(connection->GetWord()))
			{
				//Already exists this word.
				continue;
			}

			//Check if the word is in the base concept table.
			//If so, then move it to non base concept table.
			auto conceptInBaseDb = db->GetBaseConcept(0, connection->GetWord());
			if (conceptInBaseDb != NULL)
			{
				db->DeleteBaseConcept(connection->GetWord(), 0);
				db->AddNonBaseConcept(conceptInBaseDb);
			}
			else
			{
				//It is a new concept.
				auto POSs = ChinesePOSDictionary::Instance()->GetPOS(connection->GetWord());
				if (!POSs.empty())
				{
					auto word = LanguageFunc::GetParticularWord(connection->GetWord(), POSs.front());
					db->AddNonBaseConcept(word);
				}
			}
		}
	}

	db->CommitTransaction();
	delete db;
}

void AnalyzeChineseDictionary::OutputToConceptConnectionToDatabase(
	const map<string, shared_ptr<WordConnection>>& wordConnections) const
{
	ConceptDatabase *db = new ConceptDatabase(GetDatabasePath());
	db->Connect();
	db->DeleteConnectionTable();
	db->ClearConnectionOfNonBaseConcept();

	db->BeginTransaction();
	for (auto connectionPair : wordConnections)
	{
		connectionPair.second->WriteToDB(db);
	}

	db->CommitTransaction();
	delete db;
}

long WordConnection::count = 0;

void WordConnection::WriteToDB(Mind::ConceptDatabase* db)
{
	if (_connections.empty()) return;

	const int DEFAULT_ID = 0;

	//Query word from database.
	auto fromConcept = db->GetConcept(_word, DEFAULT_ID);
	if (fromConcept == NULL)
	{
		WARN_FORMAT("Cannot find word '%s' in database.", _word.c_str());
		return;
	}

	auto fromWord = fromConcept->GetWord();
	//Add to database for each connection.
	for (auto connection : _connections)
	{
		//Query word for connection word.
		auto toConcept = db->GetConcept(connection, DEFAULT_ID);
		if (toConcept == NULL)
		{
			WARN_FORMAT("Cannot find word '%s' in database.", _word.c_str());
			continue;
		}

		auto toWord = toConcept->GetWord();
		db->AddConnection(fromWord, DEFAULT_ID, toWord, DEFAULT_ID);
	}
}
