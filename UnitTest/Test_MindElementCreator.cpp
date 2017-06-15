#include "stdafx.h"
#include "Test_MindElementCreator.h"
#include "Test_DBoperator.h"

#include "../MindElement/MindElementCreator.h"
#include "../MindElement/BaseConcept.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"

using namespace CommonTool;

namespace Mind
{
	TEST(MindElementCreator, CreateBaseConceptFromDBRow)
	{
		//Create a row for base concept.
		DBRow baseRow;
		Test_DBoperator::InsertToRow("baseID", 0, baseRow);
		Test_DBoperator::InsertToRow("word", "Œ“", baseRow);
		Test_DBoperator::InsertToRow("id", 0, baseRow);
		Test_DBoperator::InsertToRow("pos", 5, baseRow);

		MindElementCreator creator;
		auto concept = creator.CreateConcept(baseRow);

		//Check type.
		ASSERT_EQ(typeid(*new BaseConcept), typeid(*concept.get()))
			<< EXPECT_RESULT(typeid(*new BaseConcept).name(), typeid(*concept.get()).name());
	}
}
