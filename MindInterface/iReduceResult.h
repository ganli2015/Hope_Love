#pragma once
#include "InOut.h"

namespace Mind
{
	class iConceptInteractTable;
	class iConcept;
}

namespace LogicSystem
{
	//////////////////////////////////////////////////////////////////////////
	///iReduceResult contains the information of reduction .
	//////////////////////////////////////////////////////////////////////////
	class _MINDINTERFACEINOUT iReduceResult: public Obj<iReduceResult>
	{
	public:
		iReduceResult(void);
		~iReduceResult(void);

		//////////////////////////////////////////////////////////////////////////
		///Get the result represented as .iConceptInteractTable.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Mind::iConceptInteractTable> GetConceptTable() const =0;

		//////////////////////////////////////////////////////////////////////////
		///Get the result of a single concept.
		//////////////////////////////////////////////////////////////////////////
		virtual shared_ptr<Mind::iConcept> GetSingleConcept() const =0;

		virtual string GetString() const;
	};
}


