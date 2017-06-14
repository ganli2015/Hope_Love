#include "stdafx.h"
#include "MockMindElementCreator.h"

#include "Matchers.h"

#include "../CommonTools/DBoperator.h"

using testing::Return;
using testing::_;
using testing::Matcher;
using ::testing::Invoke;
using ::testing::An;

namespace Mind
{
	MockMindElementCreator::MockMindElementCreator()
	{
		//Set default action.
		EXPECT_CALL(*this, CreateConcept(_))
			.WillRepeatedly(Invoke(&_real,
				static_cast<shared_ptr<Concept>(MindElementCreator::*)(const CommonTool::DBRow&)const>(&MindElementCreator::CreateConcept)));
	}


	MockMindElementCreator::~MockMindElementCreator()
	{
	}

	void MockMindElementCreator::SetCreatedConcept(const CommonTool::DBRow& dbRow, const shared_ptr<Concept> concept)
	{
		EXPECT_CALL(*this, CreateConcept(SameDBRow(dbRow))).WillRepeatedly(Return(concept));
	}

}

