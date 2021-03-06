#pragma once
#include "PublicHeader.h"

namespace Mind
{
	class iConcept;
	class BaseConcept;
}

class _UTFACILITYINOUT ConceptCreator
{
public:
	ConceptCreator(void);
	~ConceptCreator(void);

	virtual shared_ptr<Mind::iConcept> Create(const string str, const int i=0) const =0 ;
};

class _UTFACILITYINOUT SimpleConceptCreator : public ConceptCreator
{
public:
	SimpleConceptCreator(void){};
	~SimpleConceptCreator(void){};

	//////////////////////////////////////////////////////////////////////////
	//Create simple concept with word string.
	//////////////////////////////////////////////////////////////////////////
	virtual shared_ptr<Mind::iConcept> Create(const string str, const int i=0) const;

private:
	shared_ptr<Mind::BaseConcept> CreateBaseConcept(const string str, const int i = 0) const;
};

class _UTFACILITYINOUT CerebrumConceptCreator : public ConceptCreator
{
public:
	CerebrumConceptCreator(void){};
	~CerebrumConceptCreator(void){};

	//////////////////////////////////////////////////////////////////////////
	//Get concept from Cerebrum.
	//////////////////////////////////////////////////////////////////////////
	virtual shared_ptr<Mind::iConcept> Create(const string str, const int i=0) const;
};