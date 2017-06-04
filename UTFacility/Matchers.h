#pragma once
#include "PublicHeader.h"
#include <gmock/gmock.h>
#include "../Mathmatic/MathTool.h"


MATCHER_P(SameConceptTable,table,"Check if two ConceptInteractTable are the same.")
{
	if(Math::DoubleCompare(arg->Similarity(table),1)==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

MATCHER_P(SameConcept, identity, "Check if two Concept are the same.")
{
	if (arg->GetIdentity()== identity)
	{
		return true;
	}
	else
	{
		return false;
	}
}

MATCHER_P(SameDBRow, row, "Check if two dabatase rows are the same.")
{
	if (arg.GetLong("id") == row.GetLong("id") && arg.GetText("word") == row.GetText("word"))
	{
		return true;
	}
	else
	{
		return false;
	}
}