#pragma once
#include "InOut.h"

namespace Mind
{
	class iConcept;

	//////////////////////////////////////////////////////////////////////////
	///iConceptLevelTable contains a concept and distances to its all base concepts.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINTERFACEINOUT iConceptLevelTable: public Obj<iConceptLevelTable>
	{
	public:
		iConceptLevelTable(void);
		virtual ~iConceptLevelTable(void);
		
		//Get the level between the concept in <me> and the base concept <concept>.
		//If there is no base concept in <me>, return -1.
		virtual int LevelTo(const shared_ptr<iConcept> concept) const =0;
	};


}


