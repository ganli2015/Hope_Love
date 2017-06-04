#pragma once
#pragma warning(disable:4373)
#include "PublicHeader.h"
#include <gmock/gmock.h>

#include "../MindElement/Concept.h"

namespace Mind
{
	class iConceptInteractTable;

	class _UTFACILITYINOUT MockConcept : public Concept
	{
	public:
		MockConcept(const shared_ptr<DataCollection::Word> word);
		~MockConcept();

		//////////////////////////////////////////////////////////////////////////
		//Set if the EXPECT_CALL is Strict.See gtest document for detail.
		//////////////////////////////////////////////////////////////////////////
		void SetStrict(bool strict) { _isStrict = strict; }

		void AddExpectCall_AddForwardConcept(const Identity identity,const int callTimes= UNLIMITED_TIMES);
		void AddExpectCall_AddForwardModification(const Identity toConcept,const shared_ptr<iConceptInteractTable> modTable, const int callTimes = UNLIMITED_TIMES);

		MOCK_METHOD2(AddForwardConcept, void(const shared_ptr<iConcept>, const double sensitivity));
		MOCK_METHOD2(AddForwardModification, void(const shared_ptr<iConcept>, const shared_ptr<iConceptInteractTable>));
	
	private:
		Concept _real;

		bool _isStrict;
	};

}

