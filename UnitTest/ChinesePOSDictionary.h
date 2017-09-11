#pragma once
#include "PublicHeader.h"

class ChinesePOSDictionary
{
	static ChinesePOSDictionary* instance;

	//Mapping from POS string to DataCollection::PartOfSpeech.
	map<string, DataCollection::PartOfSpeech> _posMap;

	//Word and its POS
	multimap<string, DataCollection::PartOfSpeech> _word_POS;

public:

	static ChinesePOSDictionary* Instance();

	vector<DataCollection::PartOfSpeech> GetPOS(const string word) const;

private:
	ChinesePOSDictionary();
	~ChinesePOSDictionary();

	void Initialize();

	//Read file "ChineseDict.txt"
	void ReadDictionary();

	void PreparePOSMap();
};

