#pragma once
#include "InOut.h"

#include "../MindInterface/CommonFunction.h"

namespace Mind
{
	class iConcept;
	class iCerebrum;
}

class ConceptVector;

class _SENTENCEANALYSISALGORITHMINOUT FindSimilarConcept
{
	Mind::iCerebrum *_brain;
	const size_t DEFAULT_SIZEOF_RETURN = 10;

	map<int, shared_ptr<Mind::iConcept>> _index_concept;
	map<Mind::Identity, int> _concept_index;

	map<shared_ptr<Mind::iConcept>, ConceptVector> _conceptVectors;

	size_t _sizeOfResult;

	bool _builtConceptVectors;

public:
	FindSimilarConcept();
	~FindSimilarConcept();

	//Set the max size of vector returned by <Find>.
	void SetResultSize(const int val) { _sizeOfResult = val; }

	//Find similar concepts with <concept> from all concepts in mind.
	vector<shared_ptr<Mind::iConcept>> Find(const shared_ptr<Mind::iConcept> concept);

	vector<string> Find(const string word);

private:

	//Assign each concept an index to construct a Concept Vector.
	//Each concept is mapped to an index of vector.
	void PrepareIndexOfConcepts(const vector<shared_ptr<Mind::iConcept>>& concepts);

	//Construct a concept vector from concept.
	ConceptVector MappingConceptToVector(const shared_ptr<Mind::iConcept> concept) const;
	void Recursive_MappingConceptToVector(const shared_ptr<Mind::iConcept> concept, const int distance, ConceptVector& conceptVector) const;

	//Build _conceptVectors from <concepts>.
	void BuildAllConceptVectors(const vector<shared_ptr<Mind::iConcept>>& concepts);

	//Compute the similarity between all concepts and <concept>. 
	void ComputeSimilarityOfConcepts(const shared_ptr<Mind::iConcept> concept, 
		vector<pair<float, shared_ptr<Mind::iConcept>>>& similarityMap);

	//Compute the similarity between <conceptVector> and <myConceptVector>. 
	float ComputeSimilarity(const ConceptVector& conceptVector,
		const ConceptVector& myConceptVector) const;

	//Compute an weight value given two distance value.
	float ComputeDistanceDiff(const float dis1, const float dis2) const;

	//Select concepts for returned by <Find>.
	vector<shared_ptr<Mind::iConcept>> SelectConceptsForReturn(const vector<pair<float, shared_ptr<Mind::iConcept>>>& similarityMap) const;
};

//It is a Sparse vector whose dimension is size of all concepts.
//Each element contains information of a certain concept.
class ConceptVector
{
	struct ConceptInfo
	{
		shared_ptr<Mind::iConcept> concept;
		int distance;
	};

	int _size;

	map<int, ConceptInfo> _conceptMap;

public:
	ConceptVector();
	ConceptVector(const int size);
	~ConceptVector();

	void AddConcept(const int index, const shared_ptr<Mind::iConcept> concept, const int distance);
	void GetConcept(const int index, shared_ptr<Mind::iConcept>& concept, int& distance) const;

	//Get indices of elements with value.
	vector<int> GetAllNonZeroIndex() const;
};