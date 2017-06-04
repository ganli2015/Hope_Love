#include "stdafx.h"
#include "MockConcept.h"

#include "Matchers.h"

#include "../MindElement/ConceptInteractTable.h"

#include "../MindInterface/iConceptInteractTable.h"

using testing::Return;
using testing::_;
using testing::Matcher;
using ::testing::Invoke;
using ::testing::An;

namespace Mind
{
	MockConcept::MockConcept(const shared_ptr<DataCollection::Word> word):Concept(word),_real(word), _isStrict(false)
	{
		//Set default action.
// 		EXPECT_CALL(*this, AddForwardConcept(_,_))
// 			.WillRepeatedly(Invoke(&_real, &Concept::AddForwardConcept));
// 		EXPECT_CALL(*this, AddForwardModification(_, An<shared_ptr<iConceptInteractTable>>()))
// 			.WillRepeatedly(Invoke(&_real,
// 				static_cast<void (Concept::*)(const shared_ptr<iConcept>,const shared_ptr<iConceptInteractTable>)>(&Concept::AddForwardModification)));
		auto failFunc = [](...) {throw runtime_error("Uninteresting call!"); };
		
		EXPECT_CALL(*this, AddForwardConcept(_, _))
			.WillRepeatedly(Invoke(failFunc));
		EXPECT_CALL(*this, AddForwardModification(_, An<shared_ptr<iConceptInteractTable>>()))
			.WillRepeatedly(Invoke(failFunc));
	}

	MockConcept::~MockConcept()
	{
	}

	void MockConcept::AddExpectCall_AddForwardConcept(const Identity concept, const int callTimes)
	{
		EXPECT_CALL(*this, AddForwardConcept(SameConcept(concept), _)).Times(callTimes);
	}

	void MockConcept::AddExpectCall_AddForwardModification(const Identity toConcept, const shared_ptr<iConceptInteractTable> modTable, const int callTimes)
	{
		EXPECT_CALL(*this, AddForwardModification(SameConcept(toConcept), SameConceptTable(modTable))).Times(callTimes);
	}

}
