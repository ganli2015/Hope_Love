#pragma once
#include "PublicHeader.h"

#include "../Mathmatic/DirectedGraph.h"

namespace DataCollection
{
	class Word;
}

namespace Mind
{
	class iConcept;
	class ConceptDatabase;
}

namespace Math
{
	class DirectedGraph;
}


class WordMeaning
{
	string _meaning;
	vector<string> _segmentedMeaning;

public:
	WordMeaning(const string val):_meaning(val) {};
	~WordMeaning() {};

	string GetMeaning() const { return _meaning; }

	void SetSegmented(const vector<string>& val) { _segmentedMeaning = val; }

	bool HasWord(const string word)
	{
		return find(_segmentedMeaning.begin(), _segmentedMeaning.end(), word) != _segmentedMeaning.end();
	}
};


class WordDefinition
{
	string _word;
	vector<WordMeaning> _meanings;
	long _freq = 0;

	set<shared_ptr<WordDefinition>> _dependent;

public:
	WordDefinition() {};
	WordDefinition(string w) :_word(w) {};
	~WordDefinition() {};

	void AddMeaning(const string meaning) { _meanings.push_back(WordMeaning(meaning)); }

	vector<string> GetMeanings() const
	{
		vector<string> res(_meanings.size());
		transform(_meanings.begin(), _meanings.end(), res.begin(),
			[](const WordMeaning& meaning)->string {return meaning.GetMeaning(); });

		return res;
	}

	//Pass one segmented meaning to me.
	//Each string in <segment> is a word.
	void SetMeaningSegment(const string meaningStr, const vector<string>& segment)
	{
		auto existMeaning = find_if(_meanings.begin(), _meanings.end(),
			[meaningStr](const WordMeaning& meaning)->bool {return meaning.GetMeaning() == meaningStr; });

		if (existMeaning != _meanings.end())
		{
			existMeaning->SetSegmented(segment);
		}
		else
		{
			cout << "Cannot find meaning!!" << endl;
		}
	}

	//Check if there is a word in the meaning of me.
	bool HasWordInMeaning(const string word)
	{
		for (auto meaning : _meanings)
		{
			if (meaning.HasWord(word))
			{
				return true;
			}
		}

		return false;
	}
	
	string GetWord() const { return _word; };

	long Freq() const { return _freq; }
	void Freq(long val) { _freq = val; }

	void AddDependence(const shared_ptr<WordDefinition> wordDef) { _dependent.insert(wordDef); }

	void Write(ofstream& out)
	{
		out << _word << " ";
		for (auto dep : _dependent)
		{
			out << dep->GetWord() << " ";
		}
		out << endl;
	}
};

class WordConnection : public Math::IVertex
{
	static long count;

	string _word;
	vector<string> _connections;
	bool _isBase;

	const long _index;

public:

	WordConnection(const string val) :_word(val), _index(count++), _isBase(false) {};
	~WordConnection() {};

	void AddConnection(const string val) { _connections.push_back(val); }
	vector<string> GetConnections() const { return _connections; }

	string GetWord() const { return _word; }

	virtual long GetID() const
	{
		return _index;
	}

	void IsBase() { _isBase = true; }
	bool IsBaseWord() const { return _isBase; }

	//Remove invalid connections according to valid connection words.
	void RemoveInvalidConnection(const set<string>& validWords)
	{
		for (vector<string>::iterator it = _connections.begin(); it != _connections.end();)
		{
			if (validWords.find(*it) != validWords.end())
			{
				//Is valid word.
				++it;
			}
			else
			{
				//Invalid connection.
				it = _connections.erase(it);
			}
		}
	}

	void WriteToDB(Mind::ConceptDatabase* db);
};

class AnalyzeChineseDictionary
{
	//The count of base concepts for extracted word from the file.
	const int BASE_COUNT = 1000;
	//The count of base concepts for word with single character.
	const int BASE_COUNT_SINGLE_CHAR = 500;
	//The file path of extracted base words.
	const string BASE_WORD_FILE = "base_words.txt";
	//The file path of extracted base words.
	const string NON_BASE_WORD_FILE = "non_base_words.txt";

	const string WORD_CONNECTION_FILE = "word_connection.txt";

	const string VALID_CONNECTION_FILE = "valid_connection.txt";

	map<string, shared_ptr<WordDefinition>> _allWordDefs;

	//Base words in Both Chinese Dictionary and iCerebrum.
	map<string,shared_ptr<WordDefinition>> _baseWordDefs;
	//Non Base words in Both Chinese Dictionary and iCerebrum.
	map<string, shared_ptr<WordDefinition>> _nonBaseWordDefs;

public:
	AnalyzeChineseDictionary();
	~AnalyzeChineseDictionary();

	//Analyze file of Chinese dictionary and extract base words in it.
	//The base words are output to a file "non_base_words.txt" and "base_words.txt" .
	void ExtractBaseWords(const string filePath);

	//Analyze file of Chinese dictionary and read base words from the file.
	//Then try to find the connection of non base words to other words.
	//Finally output word relations to "word_connection.txt".
	//Run <ExtractBaseWords> FIRST if you have no "non_base_words.txt" and "base_words.txt".
	void BuildConnection(const string filePath);

	//Input word connection file built by <BuildConnection>.
	//Build graph of all connections and analyze the whole graph.
	//Try to eliminate sub graph with cycle and contains no base word.
	//Then collect words which have path to a base word.
	//Finally output valid words to "valid_connection.txt".
	void AnalyzeValidConnections(const string filePath);

	//Select valid connection words according to data in the file <validConnectionPath>.
	//Output connection data to the database.
	//Base concepts and Non base concepts in the database will be adjusted according to 
	//previous analysis from the Chinese dictionary.
	void OutputToDB(const string connectionPath, const string validConnectionPath);

private:

	void ExtractAllDefinitions(const string filePath);

	bool ExtractDefinition(const string line, shared_ptr<WordDefinition>& def) const;

	string FindMeaning(const string line, const string indexTag) const;

	void DistinguishBaseConceptAndNonBase();

	//Generate a map with single character string as key and word definition as value.
	//It contains all word definitions.
	multimap<string, shared_ptr<WordDefinition>> GenerateCharacterWordDefMap() const;

	//Split meaning sentences into characters.
	//No duplicated!
	set<string> GenerateCharacter(const shared_ptr<WordDefinition> line) const;

	long ComputeFreq(const string word, multimap<string, shared_ptr<WordDefinition>>& wordDefMap) const;

	bool MeaningHasWord(const shared_ptr<WordDefinition>& wordDef, const string word) const;

	//Remove example sentence.
	string ExtractMeaning(const string& line) const;

	void OutputBaseWords(const string filePath, const map<string, shared_ptr<WordDefinition>>& wordMap) const;

	//Read base or non base word from the file and append to wordMap.
	void ReadWordsFromFile(const string filePath, map<string, shared_ptr<WordDefinition>>& wordMap) const;

	//WordSegment for each meaning of each word definition.
	//The result is in the object of word definition.
	void SegmentMeanings();

	void FindConnection();

	void FindWhoDependOnMe(const string word, const shared_ptr<WordDefinition> wordDef, const multimap<string, shared_ptr<WordDefinition>>& wordDefMap);

	void OutputWordConnection() const;

	map<string, shared_ptr<WordConnection>> ReadWordConnections(const string filePath) const;

	shared_ptr<Math::DirectedGraph> BuildGraph(const map<string, shared_ptr<WordConnection>> wordConnections) const;

	//Check each of the sub graph.
	//Check if the graph does not have cycle and each word in the graph
	//has a connection path to a base word.
	//Finally return all valid connection structure of these words.
	vector<shared_ptr<WordConnection>> GetValidConnections(const vector<shared_ptr<Math::DirectedGraph>>& graphs);

	void OutputValidConnections(const vector<shared_ptr<WordConnection>>& wordConnections) const;


	set<string> ReadValidConnectionWords(const string filePath) const;


	//Remove invalid key words in the <wordConnections>.
	//And invalid connection words in the WordConnection will be removed as well.
	void RemoveInvalidWords(const set<string>& validWords, map<string, shared_ptr<WordConnection>>& wordConnections) const;

	//Check base concepts and non base concepts in the database.
	//According to <wordConnections>, move the concepts to the right table.
	void RefactorConceptInDatabase(const map<string, shared_ptr<WordConnection>>& wordConnections) const;

	//Output connection to hope_love database
	void OutputToConceptConnectionToDatabase(const map<string, shared_ptr<WordConnection>>& wordConnections) const;


};

