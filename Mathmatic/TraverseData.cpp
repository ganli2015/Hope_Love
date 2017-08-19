#include "stdafx.h"
#include "TraverseData.h"

#include "DirectedGraph.h"

namespace Math
{
	TraverseData::TraverseData(const SearchResult res, const shared_ptr<IVertex> vert, const DirectedGraph* graph):
		_startVert(vert)
	{
		_edges = res.GetEdges();
		_distanceMap = res.GetDistanceMap();
		_parentMap = res.GetParentMap();

		for (auto vert : graph->GetAllVertices())
		{
			_vertexMap[vert->GetID()] = vert;
		}
	}


	TraverseData::~TraverseData()
	{
	}

	bool TraverseData::HasConnection(const shared_ptr<IVertex> to) const
	{
		return _parentMap.find(to->GetID()) != _parentMap.end();
	}

	list<shared_ptr<IVertex>> TraverseData::GetPathTo(const shared_ptr<IVertex> to) const
	{
		list<shared_ptr<IVertex>> res;
		if (!HasConnection(to)) return res;

		res.push_front(to);
		//Search from vertex <to> to start vertex.
		long currentID = to->GetID();
		do
		{
			long parentID = _parentMap.at(currentID);
			auto parentVert = _vertexMap.at(parentID);
			res.push_front(parentVert);
			currentID = parentID;

		} while (currentID != _startVert->GetID());

		//Check if the first vertex equals to the start vertex.
		Check(_startVert == res.front());

		return res;
	}

	bool TraverseData::HasCycle(list<shared_ptr<IVertex>>& cycle) const
	{
		if (!HasConnection(_startVert)) return false;

		//Get the parent of start.
		long parentID = _parentMap.at(_startVert->GetID());
		auto parentVert = _vertexMap.at(parentID);

		//Get path to the parent.
		cycle = GetPathTo(parentVert);
		//Push back the start vertex to make a cycle.
		cycle.push_back(_startVert);

		return true;
	}

	shared_ptr<DirectedGraph> TraverseData::GenerateGraph() const
	{
		auto allEdges = this->_edges;

		shared_ptr<DirectedGraph> res(new DirectedGraph(allEdges.size() * 2));
		res->AddVertex(this->_startVert);
		for (auto edge : allEdges)
		{
			auto fromVert = _vertexMap.at(edge.first);
			auto toVert = _vertexMap.at(edge.second);

			res->AddEdge(fromVert, toVert);
		}

		return res;
	}

	vector<shared_ptr<IVertex>> TraverseData::GetAllConnectVertice() const
	{
		vector<shared_ptr<IVertex>> res;

		for (auto disPair : _distanceMap)
		{
			auto vert = _vertexMap.at(disPair.first);
			res.push_back(vert);
		}

		return res;
	}

}

