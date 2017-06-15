#pragma once
#pragma warning(disable:4373)
#include "PublicHeader.h"
#include <gmock/gmock.h>

#include "../MindElement/MindElementCreator.h"

namespace Mind
{
	class _UTFACILITYINOUT MockMindElementCreator : public MindElementCreator
	{
	public:
		MockMindElementCreator();
		~MockMindElementCreator();

		//////////////////////////////////////////////////////////////////////////
		//Set created concept by calling <CreateConcept>.
		//////////////////////////////////////////////////////////////////////////
		void SetCreatedConcept(const CommonTool::DBRow& dbRow,const shared_ptr<Concept> concept);

		MOCK_CONST_METHOD1(CreateConcept, shared_ptr<Concept>(const  CommonTool::DBRow&));

	private:
		MindElementCreator _real;
	};
}


