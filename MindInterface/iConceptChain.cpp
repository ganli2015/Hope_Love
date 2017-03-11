#include "StdAfx.h"
#include "iConceptChain.h"
#include "iConcept.h"
#include "CommonFunction.h"

namespace Mind
{
	iConceptChain::iConceptChain(void)
	{
	}


	iConceptChain::~iConceptChain(void)
	{
	}

	std::string iConceptChain::GetString() const
	{
		string res = "";

		auto conceptVec = GetConceptVec();
		for (unsigned int i=0;i<conceptVec.size();++i)
		{
			auto conceptID = conceptVec[i]->GetIdentity();
			res += conceptID.id + " " + conceptID.str;
			if (i != conceptVec.size() - 1)
			{
				res += ",";
			}
		}

		return res;
	}

}

