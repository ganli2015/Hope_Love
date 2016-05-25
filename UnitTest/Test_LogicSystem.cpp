#include "StdAfx.h"
#include "Test_LogicSystem.h"
#include "Test_iRelation.h"

#include "../LogicSystem/Logic.h"
#include "../LogicSystem/CompositeExpression.h"
#include "../LogicSystem/SingleExpression.h"
#include "../LogicSystem/RelationLeaf.h"
#include "../LogicSystem/RelationNode.h"
#include "../LogicSystem/Arbitrariness.h"
#include "../LogicSystem/Symbol.h"
#include "../LogicSystem/LogicStatement.h"
#include "../LogicSystem/Inequality.h"
#include "../LogicSystem/Number.h"

#include "../Mind/Cerebrum.h"

#include "../MindElement/ConceptInteractTable.h"
#include "../MindElement/Concept.h"

#include "../MindInterface/iCerebrum.h"
#include "../MindInterface/CommonFunction.h"
#include "../MindInterface/iDeduceResult.h"

#include "../DataCollection/GrammaPattern.h"

#include "FuncForTest.h"
#include "ConceptTableCreator.h"

using namespace DataCollection;
using namespace Mind;
using namespace LogicSystem;
using namespace FuncForTest;

typedef Arbitrariness<iConcept> Arb;
typedef LogicType::ConSymbol ConSymbol;
typedef Symbol<iConcept> Sym;

typedef AddPatternToCerebrum Test_Logic;
typedef InitCerebrum Test_Number;
typedef InitCerebrum Test_iLogicStatement;

TEST_F(Test_Logic,Determine)
{
	shared_ptr<RelationNode> conditionRel(new RelationNode());
	shared_ptr<RelationLeaf> resultRel(new RelationLeaf());
	iRelationSample::RelationPair(conditionRel,resultRel);

	//Create logic statement
	shared_ptr<LogicStatement> statement(new LogicStatement(conditionRel,resultRel));

	iCerebrum* brain=iCerebrum::Instance();
	brain->AddLogicStatement(statement);

	Logic logic;

	shared_ptr<CompositeExpression> condition(new CompositeExpression());
	condition->AddExpression("二大于一");
	condition->AddExpression("三大于二");

	shared_ptr<iExpression> conclusion_true(new SingleExpression("三大于一"));
	shared_ptr<iExpression> conclusion_false(new SingleExpression("一大于三"));

	ASSERT_TRUE(logic.Determine(condition,conclusion_true)==True);
	ASSERT_TRUE(logic.Determine(condition,conclusion_false)==False);
}

TEST_F(Test_Number,Match)
{
	Identity iden;
	iden.str="二";
	iden.id=0;
	shared_ptr<iConcept> er=iCerebrum::Instance()->GetConcept(iden);

	shared_ptr<Number<iConcept>> num=Number<iConcept>::Create();

	ASSERT_TRUE(num->Match(er));
}

TEST_F(Test_iLogicStatement,Deduce)
{
	///Construct a logicStatment.
	///Input: "二-加,加-三",
	///Output: "二-加,加-一,三-次,次-加".
	shared_ptr<RelationLeaf> conditionRel(new RelationLeaf());
	shared_ptr<RelationLeaf> resultRel(new RelationLeaf());
	iRelationSample::RelationPair2(conditionRel,resultRel);

	shared_ptr<iLogicStatement> logicStatment(new LogicStatement(conditionRel,resultRel));

	string tableStr="二-加,加-三";
	shared_ptr<iConceptInteractTable> table=ConceptTableCreator::Create(tableStr);
	shared_ptr<Stub_Expression> expre(new Stub_Expression(table));

	shared_ptr<iDeduceResult> result=logicStatment->Deduce(expre);

	string resTableStr="二-加,加-一,三-次,次-加";
	shared_ptr<iConceptInteractTable> expectTable=ConceptTableCreator::Create(resTableStr);
	shared_ptr<Stub_Expression> expect(new Stub_Expression(expectTable));

	ASSERT_TRUE(result->Matching(expect)==1);
}