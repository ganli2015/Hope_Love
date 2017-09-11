#include "stdafx.h"
#include "ChinesePOSDictionary.h"

#include "../DataCollection/Word.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"
#include "../CommonTools/LogWriter.h"

using namespace DataCollection;

ChinesePOSDictionary* ChinesePOSDictionary::instance = NULL;

ChinesePOSDictionary* ChinesePOSDictionary::Instance()
{
	if (instance == NULL)
	{
		instance = new ChinesePOSDictionary();
		instance->Initialize();
	}

	return instance;
}

vector<DataCollection::PartOfSpeech> ChinesePOSDictionary::GetPOS(const string word) const
{
	vector<DataCollection::PartOfSpeech> res = CommonTool::FindAll(word, _word_POS);
	return res;
}

ChinesePOSDictionary::ChinesePOSDictionary()
{
}


ChinesePOSDictionary::~ChinesePOSDictionary()
{
}

void ChinesePOSDictionary::Initialize()
{
	PreparePOSMap();
	ReadDictionary();
	LOG("ChinesePOSDictionary is initialized.");
}

void ChinesePOSDictionary::ReadDictionary()
{
	ifstream in("E:\\Artificial Intelligence\\Document\\DataBase\\ChineseDict.txt");

	string line = "";
	while (getline(in, line))
	{
		auto split = CommonTool::SplitString(line, ',');
		//Add POS to _word_POS
		string word = split.front();
		for (int i = 1; i < split.size(); ++i)
		{
			auto POSStr = split[i];
			if (_posMap.find(POSStr) != _posMap.end())
			{
				auto POS = _posMap[POSStr];
				_word_POS.insert(make_pair(word, POS));
			}
		}
	}
}

void ChinesePOSDictionary::PreparePOSMap()
{
	_posMap["n"] = Noun;
	_posMap["nr"] = Noun;
	_posMap["ns"] = Noun;
	_posMap["nt"] = Noun;
	_posMap["nz"] = Noun;
	_posMap["vn"] = Noun;
	_posMap["v"] = Verb;
	_posMap["vg"] = Verb;
	_posMap["i"] = Adjective;
	_posMap["a"] = Adjective;
	_posMap["m"] = Numeral;
	_posMap["q"] = Quantifier;
	_posMap["r"] = Pronoun;
	_posMap["d"] = Adverb;
	_posMap["t"] = Adverb;
	_posMap["vd"] = Adverb;
	_posMap["p"] = Preposition;
	_posMap["c"] = Conjunction;
	_posMap["u"] = Auxiliary;
	_posMap["o"] = Onomatopoeia;
	_posMap["y"] = Interjection;
	_posMap["e"] = Interjection;
}
