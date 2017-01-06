#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <HopeLoveAPI.h>


using namespace std;


class ExtractChainFromSentence
{
	struct ConversationPair
	{
		string first;
		string second;
	};

public:
	ExtractChainFromSentence();
	~ExtractChainFromSentence();

	void Run(const string filename);

private:
	void OutputChainsAndReactChains(const string filename, const vector<ExtractChainFromSentence::ConversationPair>& pairs);
	void WriteChain(const std::vector<std::pair<std::string, int>>& chain, ofstream& out);
	vector<ExtractChainFromSentence::ConversationPair> ReadConversationPairs(const string filename);
	void RemoveInvalidConversationPairs(vector<ConversationPair>& conversationPairs) const;
	bool IsValidSentence(const string line) const;
};

