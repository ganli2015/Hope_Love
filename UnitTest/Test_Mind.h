#pragma once
#include "PublicHeader.h"

namespace Mind
{
	class iConceptInteractTable;
	class LogicKnowledgeInitializer;
	class ConceptSet;
	struct Connection_Info;
}

namespace LogicSystem
{
	class iRelation;
	class iLogicStatement;
}

class TiXmlNode;

class Test_Mind
{
public:

	static shared_ptr<LogicSystem::iRelation> ParseRelation(const TiXmlNode * node,Mind::LogicKnowledgeInitializer& initer);
	static shared_ptr<LogicSystem::iLogicStatement> ParseLogicStatement(const TiXmlNode * node,Mind::LogicKnowledgeInitializer& initer);
	static Mind::Connection_Info ParseStrToConnectionInfo(const string line,const Mind::ConceptSet* conceptSet);
};
