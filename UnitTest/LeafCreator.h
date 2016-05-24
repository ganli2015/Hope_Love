#pragma once
#include "PublicHeader.h"
#include "../MindInterface/PublicTypedef.h"

namespace LogicSystem
{
	class RelationLeaf;
}

class LeafCreator
{
private:

	class iSpecialSymbol
	{
	public:
		virtual shared_ptr<LogicType::ConSymbol> Create()=0;
	};

	template<class T>
	class SpecialSymbol : public iSpecialSymbol
	{
	public:
		SpecialSymbol(){}
		shared_ptr<LogicType::ConSymbol> Create(){return T::Create();}
	};

private:
	///Restore special symbol creators
	static map<string,iSpecialSymbol*> SpSym;

public:
	LeafCreator(void);
	~LeafCreator(void);

	static void Init();
	static shared_ptr<LogicSystem::RelationLeaf> Create(const string str);

private:
	static bool Contain(const string raw,const string search);
	static shared_ptr<LogicType::ConSymbol> ParseSpSymbol(const string str);
	static shared_ptr<LogicType::ConSymbol> ParseSimpleSymbol(const string str);
};

