#include "Stdafx.h"
#include "FilePath.h"

namespace Mind
{
	static std::string DataDir = "HopeLoveData\\large\\";

	std::string GetHopeLoveMindPath()
	{
// #ifdef _Unit_Test
// 		return "E:\\Artificial Intelligence\\Project\\Hope_Love\\Mind\\HopeLove\\";
// #endif
		return DataDir;
	}

	void SetHopeLoveMindPath(const std::string str)
	{
		DataDir = str;
	}

	std::string GetDatabasePath()
	{
		return GetHopeLoveMindPath()+"data.db";
	}

}

