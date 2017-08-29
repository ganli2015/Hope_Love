#pragma once
#include "InOut.h"

namespace Mind
{
	class iConcept;
	class iCerebrum;
}

class _SENTENCEANALYSISALGORITHMINOUT FindSimilarConcept
{
	Mind::iCerebrum *_brain;

public:
	FindSimilarConcept();
	~FindSimilarConcept();

	vector<shared_ptr<Mind::iConcept>> Find(const shared_ptr<Mind::iConcept> concept);
};

