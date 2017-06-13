#include "StdAfx.h"
#include "ConfigureInfoManager.h"
#include "CommonStringFunction.h"
#include "GeneralFunctor.h"

#include <fstream>

namespace CommonTool
{

	shared_ptr<ConfigureInfoManager> ConfigureInfoManager::_instance;

	ConfigureInfoManager::ConfigureInfoManager(void):_configureFilename("config.ini")
	{
	}


	ConfigureInfoManager::~ConfigureInfoManager(void)
	{
	}

	void ConfigureInfoManager::UpdateConfigure()
	{
		_infos.clear();

		ifstream in(_configureFilename);

		string line="";
		while(getline(in,line))
		{
			if (line.find('=') == string::npos) continue;

			CommonTool::TrimBeginEndBlank(line);
			auto split = SplitString(line, '=');

			ConfigureInfo info;
			info.var = split[0];
			info.value = split[1];
			_infos.push_back(info);
		}

		in.close();
	}

	bool ConfigureInfoManager::IsTagOn( const string tag ) const
	{
		ConfigureInfo outInfo;
		return FindTagInfo(tag, outInfo);
	}

	std::string ConfigureInfoManager::GetValue(const string tag) const
	{
		ConfigureInfo outInfo;
		if (FindTagInfo(tag, outInfo))
		{
			return outInfo.value;
		}
		else
		{
			return "";
		}
	}

	bool ConfigureInfoManager::FindTagInfo(const string tag, ConfigureInfo& outInfo) const
	{
		CREATE_FUNCTOR_IR(SameCfgInfo, string, ConfigureInfo, bool,
			if (input.var == _init)
			{
				return true;
			}
			else
			{
				return false;
			}
		);

		vector<ConfigureInfo>::const_iterator sameIt = find_if(_infos.begin(), _infos.end(), SameCfgInfo(tag));
		if (sameIt == _infos.end())
		{
			return false;
		}
		else
		{
			outInfo = *sameIt;
			return true;
		}
	}

}

