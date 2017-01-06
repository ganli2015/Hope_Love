#include "stdafx.h"
#include "ExtractChainFromSentence.h"

#include "CommonTools.h"

#include <iostream>
#include <functional>
#include <algorithm>

const char ioTag= '&';
const char conceptTag= '#';
const char idStrTag= ' ';

ExtractChainFromSentence::ExtractChainFromSentence()
{
}


ExtractChainFromSentence::~ExtractChainFromSentence()
{
}

void ExtractChainFromSentence::Run(const string filename)
{
	try
	{
		HopeLove::InitializeBrain("..\\Data\\");

	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
// 	auto result = HopeLove::ParseSentence("又要买手机啦");
// 	auto chains = HopeLove::GenerateConceptChain("又要买手机啦");

// 	for (unsigned int i=0;i<result.size();++i)
// 	{
// 		cout << result[i].first << " " << result[i].second << endl;
// 	}

// 	for (unsigned int i=0;i<chains.size();++i)
// 	{
// 		cout << "chain " << i << " ";
// 		for (unsigned int j=0;j<chains[i].size();++j)
// 		{
// 			cout << chains[i][j].first << " " << chains[i][j].second << " ";
// 		}
// 
// 		cout << endl;
// 	}

	auto convPairs = ReadConversationPairs(filename);
	RemoveInvalidConversationPairs(convPairs);
	OutputChainsAndReactChains(outdir+"ReactorIO_Initial.txt", convPairs);
}

void ExtractChainFromSentence::OutputChainsAndReactChains(const string outFile,const vector<ExtractChainFromSentence::ConversationPair>& pairs)
{
	ofstream out(outFile);

	for (unsigned int i=0;i<pairs.size();++i)
	{
		auto chain = HopeLove::GenerateConceptChain(pairs[i].first);
		auto react = HopeLove::GenerateConceptChain(pairs[i].second);

		for (unsigned int j=0;j<chain.size();++j)
		{
			for (unsigned int k = 0; k < react.size(); ++k)
			{
				WriteChain(chain[j], out);
				out << ioTag;
				WriteChain(react[k], out);
				out << endl;
			}
		}
	}
}

void ExtractChainFromSentence::WriteChain(const std::vector<std::pair<std::string, int>>& chain, ofstream& out)
{
	for (unsigned int i=0;i<chain.size();++i)
	{
		out << chain[i].second << idStrTag << chain[i].first;
		if (i != chain.size() - 1)
		{
			out << conceptTag;
		}
	}
}

vector<ExtractChainFromSentence::ConversationPair> ExtractChainFromSentence::ReadConversationPairs(const string filename)
{
	vector<ConversationPair> _conversationPairs;

	_conversationPairs.clear();

	ifstream in(filename);

	string first, second;
	while (getline(in, first) && getline(in, second))
	{
		ConversationPair pair;
		pair.first = first;
		pair.second = second;

		_conversationPairs.push_back(pair);
	}

	in.close();

	return _conversationPairs;
}

void ExtractChainFromSentence::RemoveInvalidConversationPairs(vector<ConversationPair>& conversationPairs) const
{
	for (vector<ConversationPair>::iterator it = conversationPairs.begin(); it != conversationPairs.end();)
	{
		if ((!IsValidSentence(it->first)) || (!IsValidSentence(it->second)))
		{
			it = conversationPairs.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool ExtractChainFromSentence::IsValidSentence(const string line) const
{
	return line.size() % 2 == 0 && all_of(line.begin(), line.end(), bind2nd(less<int>(), 0));
}
