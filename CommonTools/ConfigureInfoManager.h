#pragma once
#include "InOut.h"

namespace CommonTool
{
	//////////////////////////////////////////////////////////////////////////
	//Store configure information from configure file.
	//The config file should be something like that:
	// TEST=1
	// TEST2=2
	//ie, use '='  to assign value to a variable.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT ConfigureInfoManager
	{
		static shared_ptr<ConfigureInfoManager> _instance;

		const string _configureFilename;

		struct ConfigureInfo
		{
			string var;
			string value;
		};

		vector<ConfigureInfo> _infos;

	public:
		ConfigureInfoManager(void);
		~ConfigureInfoManager(void);

		//Get unique instance of ConfigureInfoManager.
		static shared_ptr<ConfigureInfoManager> GetInstance() 
		{
			if(_instance==NULL)
			{
				_instance=shared_ptr<ConfigureInfoManager>(new ConfigureInfoManager());
			}
			return _instance;
		}

		//Read from configure file again and refresh configure information in <me>.
		void UpdateConfigure();
		//Check whether there is configure information <tag> in <me>.
		bool IsTagOn(const string tag) const;

		string GetValue(const string tag) const;

	private:
		
		bool FindTagInfo(const string tag, ConfigureInfo& outInfo) const;
	};
}

///If there is <tag> in the configure file ,then do...
#define CFG_SECTION(tag) if(CommonTool::ConfigureInfoManager::GetInstance()->IsTagOn(#tag))
///If there is <tag> in the configure file ,then do <trueBody>,otherwise <falseBody>.
#define CFG_IF(tag,trueBody,falseBody) \
	if(CommonTool::ConfigureInfoManager::GetInstance()->IsTagOn(#tag)) \
	{ \
		trueBody \
	} \
	else \
	{ \
		falseBody \
	} 

