#include "StdAfx.h"
#include "iConceptInteractTable.h"
#include "iConcept.h"

namespace Mind
{
	iConceptInteractTable::iConceptInteractTable()
	{
	}


	iConceptInteractTable::~iConceptInteractTable(void)
	{
	}

	std::string iConceptInteractTable::GetString() const
	{
		string res="";

		vector<MindType::ConceptPair> pairs=GetAllRelations();
		for (size_t i=0;i<pairs.size();++i)
		{
			string firstStr = pairs[i].first->GetString();
			string secondStr = pairs[i].second->GetString();
			res += firstStr +" " + secondStr + "\n";
		}

		return res;
	}

}

