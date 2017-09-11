#include "stdafx.h"
#include "FindSimilarConcept.h"

#include "../MindInterface/iCerebrum.h"
#include "../MindInterface/iConcept.h"

#include "../CommonTools/LogWriter.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/Common.h"

using namespace Mind;
using namespace CommonTool;

FindSimilarConcept::FindSimilarConcept() :_sizeOfResult(DEFAULT_SIZEOF_RETURN), _builtConceptVectors(false)
{
	_brain = iCerebrum::Instance();
}


FindSimilarConcept::~FindSimilarConcept()
{
}

vector<shared_ptr<Mind::iConcept>> FindSimilarConcept::Find(const shared_ptr<Mind::iConcept> concept)
{
	NDC_SECTION("FindSimilarConcept");

	if (!_builtConceptVectors)
	{
		vector<shared_ptr<Mind::iConcept>> allConcepts = _brain->GetAllConcepts();
		LOG("Finish read all concepts.");
		PrepareIndexOfConcepts(allConcepts);
		DEBUG_FORMAT2("Size of _index_concept : %d ; Size of _concept_index : %d", _index_concept.size(), _concept_index.size());

		BuildAllConceptVectors(allConcepts);
		DEBUGLOG("Finish build concept vectors.");

		_builtConceptVectors = true;
	}

	vector<pair<float, shared_ptr<Mind::iConcept>>> similarityMap;
	ComputeSimilarityOfConcepts(concept, similarityMap);

	return SelectConceptsForReturn(similarityMap);
}

vector<string> FindSimilarConcept::Find(const string word)
{
	vector<string> res;
	auto concepts = _brain->GetConcepts(word);
	LOG_FORMAT("Size of concepts: %d", concepts.size());
	for (auto concept : concepts)
	{
		DEBUG_FORMAT("Concept: %s.", concept->Print());

		auto similar = Find(concept);

		DEBUG_FORMAT("Size of similar concepts: %d.", similar.size());

		for (auto similarConcept :similar)
		{
			res.push_back(similarConcept->GetString());
		}
	}

	return res;
}

void FindSimilarConcept::PrepareIndexOfConcepts(const vector<shared_ptr<Mind::iConcept>>& concepts)
{
	_index_concept.clear();
	_concept_index.clear();

	for (size_t i = 0; i < concepts.size(); ++i)
	{
		_index_concept[i] = concepts[i];
		_concept_index[concepts[i]->GetIdentity()] = i;
	}
}

ConceptVector FindSimilarConcept::MappingConceptToVector(const shared_ptr<Mind::iConcept> concept) const
{
	ConceptVector conceptVector(_index_concept.size());
	auto forwardConcepts = concept->GetForwardConcepts();
	for (auto forward : forwardConcepts)
	{
		//Recursively search forward concepts.
		//The distance between forward concept and current concept is 1.
		Recursive_MappingConceptToVector(forward, 1, conceptVector);
	}

	return conceptVector;
}

void FindSimilarConcept::Recursive_MappingConceptToVector(const shared_ptr<Mind::iConcept> concept, const int distance, ConceptVector& conceptVector) const
{
	auto index = _concept_index.at(concept->GetIdentity());
	conceptVector.AddConcept(index, concept, distance);

	auto forwardConcepts = concept->GetForwardConcepts();
	for (auto forward : forwardConcepts)
	{
		int newDistance = distance + 1;
		Recursive_MappingConceptToVector(forward, newDistance, conceptVector);
	}
}

void FindSimilarConcept::BuildAllConceptVectors(const vector<shared_ptr<Mind::iConcept>>& concepts)
{
	_conceptVectors.clear();
	for (auto concept : concepts)
	{
		auto conceptVector = MappingConceptToVector(concept);
		_conceptVectors[concept] = conceptVector;
	}
}

void FindSimilarConcept::ComputeSimilarityOfConcepts(const shared_ptr<Mind::iConcept> concept, 
	vector<pair<float, shared_ptr<Mind::iConcept>>>& similarityMap)
{
	similarityMap.clear();

	auto myConceptVector = MappingConceptToVector(concept);

	DataCollection::PartOfSpeech myPOS = concept->GetPartOfSpeech();
	for (auto conceptVector : _conceptVectors)
	{
		//Filter words which have different POS.
		auto pos = conceptVector.first->GetPartOfSpeech();
		if (pos != myPOS)
		{
			continue;
		}

		NDC_SECTION(conceptVector.first->GetString().c_str());
		float simi = ComputeSimilarity(conceptVector.second, myConceptVector);
		if (simi != 0.f)
		{
			DEBUG_FORMAT2("'%s' has some similarity: %f", conceptVector.first->Print(), simi);
			similarityMap.push_back(make_pair(simi, conceptVector.first));
		}
	}

	LOG("Finish computing similarity.");

	//Sort by similarity from large to small.
	sort(similarityMap.begin(), similarityMap.end(),
		[](pair<float, shared_ptr<Mind::iConcept>> p1, pair<float, shared_ptr<Mind::iConcept>> p2) ->
		bool { return p1.first > p2.first; });
}

float FindSimilarConcept::ComputeSimilarity(const ConceptVector& conceptVector,
	const ConceptVector& myConceptVector) const
{
	auto nonZeroIndex = conceptVector.GetAllNonZeroIndex();
	auto myNonZeroIndex = myConceptVector.GetAllNonZeroIndex();
	auto sameIndices = FindSameInVectors(myNonZeroIndex, nonZeroIndex);

	float res = 0.f;
	for (auto sameIndex : sameIndices)
	{
		//Compute each distance of the same concept and then compute difference of distances.
		shared_ptr<iConcept> concept;
		int distance;
		conceptVector.GetConcept(sameIndex, concept, distance);

		shared_ptr<iConcept> myConcept;
		int myDistance;
		myConceptVector.GetConcept(sameIndex, myConcept, myDistance);

		auto diff = ComputeDistanceDiff(distance, myDistance);

		DEBUG_FORMAT("Same concept: %s", myConcept->Print());

		res += diff;
	}

	return res;
}

float FindSimilarConcept::ComputeDistanceDiff(const float dis1, const float dis2) const
{
	return (float)exp(-abs(dis1 - dis2));
}

vector<shared_ptr<Mind::iConcept>> FindSimilarConcept::SelectConceptsForReturn(
	const vector<pair<float, shared_ptr<Mind::iConcept>>>& similarityMap) const
{
	size_t sizeOfResult = min(similarityMap.size(), _sizeOfResult);

	vector<pair<float, shared_ptr<Mind::iConcept>>> returnedSimiMap(similarityMap.begin(), similarityMap.begin() + sizeOfResult);
	vector<shared_ptr<Mind::iConcept>> res;
	for (auto simiPair : returnedSimiMap)
	{
		res.push_back(simiPair.second);
	}

	return res;
}

ConceptVector::ConceptVector(const int size) :_size(size)
{

}

ConceptVector::ConceptVector() : _size(0)
{

}

ConceptVector::~ConceptVector()
{

}

void ConceptVector::AddConcept(const int index, const shared_ptr<Mind::iConcept> concept, const int distance)
{
	if (index > _size - 1)
	{
		throw invalid_argument(StringFormat("index should be less than size. index: %d; size: %d", index, _size));
	}

	if (_conceptMap.find(index) != _conceptMap.end())
	{
		//throw invalid_argument(StringFormat("index already exists. index: %d; concept: %s", index, concept->Print()));
	}

	ConceptInfo info;
	info.concept = concept;
	info.distance = distance;
	_conceptMap[index] = info;
}

void ConceptVector::GetConcept(const int index, shared_ptr<Mind::iConcept>& concept, int & distance) const
{
	auto info = _conceptMap.at(index);
	concept = info.concept;
	distance = info.distance;
}

vector<int> ConceptVector::GetAllNonZeroIndex() const
{
	vector<int> res;
	for (auto pair : _conceptMap)
	{
		res.push_back(pair.first);
	}

	return res;
}

