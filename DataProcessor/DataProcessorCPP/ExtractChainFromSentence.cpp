#include "stdafx.h"
#include "ExtractChainFromSentence.h"

#include <iostream>

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
	auto result = HopeLove::ParseSentence("��Ҫ���ֻ���");

	for (unsigned int i=0;i<result.size();++i)
	{
		cout << result[i].first << " " << result[i].second << endl;
	}
}
