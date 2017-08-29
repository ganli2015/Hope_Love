#include "stdafx.h"
#include "FindSimilarConcept.h"

#include "../MindInterface/iCerebrum.h"
#include "../MindInterface/iConcept.h"

using namespace Mind;

FindSimilarConcept::FindSimilarConcept()
{
	_brain = iCerebrum::Instance();
}


FindSimilarConcept::~FindSimilarConcept()
{
}

vector<shared_ptr<Mind::iConcept>> FindSimilarConcept::Find(const shared_ptr<Mind::iConcept> concept)
{
	//concept->
	return vector<shared_ptr<Mind::iConcept>>();
}
