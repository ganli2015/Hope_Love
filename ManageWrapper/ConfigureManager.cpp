#include "StdAfx.h"
#include "ConfigureManager.h"

#include "../Mind/Cerebrum.h"
#include "../Mind/GrammarFeatureDatabase.h"

#include "../MindElement/MindElementCreator.h"

#include "../LogicSystem/LogicElementCreator.h"
#include "../LogicSystem/LogicKnowledge.h"
#include "../LogicSystem/LogicKnowledgeInitializer.h"

#include "../CommonTools/ConfigureInfoManager.h"
#include "../CommonTools/LogWriter.h"

namespace ManageWrapper
{
	ConfigureManager::ConfigureManager(void)
	{
	}

	void ConfigureManager::Configure()
	{
		LOG("--------------------------Start-----------------------");
		///ConfigureInfoManager
		shared_ptr<CommonTool::ConfigureInfoManager> cfgInfo=CommonTool::ConfigureInfoManager::GetInstance();
		cfgInfo->UpdateConfigure();

		///MindElementCreator
		shared_ptr<Mind::MindElementCreator> mindElementCreator(new Mind::MindElementCreator());
		Mind::iMindElementCreator::SetImp(mindElementCreator);

		///LogicElementCreator
		shared_ptr<LogicSystem::LogicElementCreator> logicElementCreator(new LogicSystem::LogicElementCreator());
		LogicSystem::iLogicElementCreator::SetImp(logicElementCreator);

		///iCerebrum
		Mind::Cerebrum *brain=Mind::Cerebrum::Instance();
		Mind::iCerebrum::SetInstance(brain);

		///iLogicKnowledge
		LogicSystem::LogicKnowledge* knowledge=new LogicSystem::LogicKnowledge();
		LogicSystem::LogicKnowledgeInitializer knowledgeInit;
		knowledgeInit.Initialize("HopeLoveData\\LogicStatements.txt",knowledge);
		LOG("Initialized LogicKnowledge.");
		brain->SetLogicKnowledge(knowledge);

	}
}


