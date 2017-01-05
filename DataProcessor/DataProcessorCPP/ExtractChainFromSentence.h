#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <HopeLoveAPI.h>


using namespace std;


class ExtractChainFromSentence
{
public:
	ExtractChainFromSentence();
	~ExtractChainFromSentence();

	void Run(const string filename);

};

