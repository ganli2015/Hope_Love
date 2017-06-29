#include "stdafx.h"
#include "DirectedGraph.h"
#include "BoostGraphInternal.h"

#include "../CommonTools/assertions.h"

namespace Math
{

	DirectedGraph::DirectedGraph(const size_t vertNum):_maxInnerID(0)
	{
		_graph = new GraphImp(vertNum);
	}


	DirectedGraph::~DirectedGraph()
	{
		if(_graph!=NULL)
			delete _graph;
	}

	void DirectedGraph::AddEdge(const shared_ptr<IVertex> from, const shared_ptr<IVertex> to)
	{
		auto fromID = from->GetID(), toID = to->GetID();
		assert(fromID != toID);

		//Get inner ID from Vertex ID.
		auto innerFromID = TryGetInnerID(from);
		auto innerToID = TryGetInnerID(to);
		boost::add_edge(innerFromID, innerToID, EdgeProperty(innerFromID, innerToID), *_graph);
		//Set IVertex to property.
		GetVertextProperty(fromID).vert = from;
		GetVertextProperty(toID).vert = to;
	}

	vector<shared_ptr<IVertex>> DirectedGraph::GetAdjacentVertices(const long id) const
	{
		auto adjIDs = QueryAdjacentVertices(id);
		vector<shared_ptr<IVertex>> res;
		for (auto id : adjIDs)
		{
			res.push_back(GetVertextProperty(id).vert);
		}

		return res;
	}

	DirectedGraph::VertexProperty& DirectedGraph::GetVertextProperty(const long id) const
	{
		auto innerID = GetInnerID(id);
		return (*_graph)[innerID];
	}

	bool DirectedGraph::HasCycle() const
	{
		bool has_cycle = false;
		internal::cycle_detector vis(has_cycle);
		boost::depth_first_search(*_graph, visitor(vis));

		return has_cycle;
	}

	bool DirectedGraph::HasCycle(const shared_ptr<IVertex> vert) const
	{
		//FIXME: 
		//Generate a sub graph containing <vert>.
		auto subGraph = GenerateSubGraph(vert);

		return subGraph->HasCycle();
	}

	DirectedGraph::EdgeSet DirectedGraph::GetAllConnectedEdges(const long id) const
	{
		GraphSearchEngine searchEngine;
		auto result = searchEngine.BFS(this, id);

		return result.GetEdges();
	}

	shared_ptr<DirectedGraph> DirectedGraph::GenerateSubGraph(const shared_ptr<IVertex> vert) const
	{
		auto allConnectedEdges = GetAllConnectedEdges(vert->GetID());

		//Add all edges to the returned graph.
		shared_ptr<DirectedGraph> res(new DirectedGraph(allConnectedEdges.size() * 2));
		for (auto edge : allConnectedEdges)
		{
			auto fromVert = GetVertextProperty(edge.first).vert;
			auto toVert = GetVertextProperty(edge.second).vert;

			res->AddEdge(fromVert, toVert);
		}

		return res;
	}

	vector<long> DirectedGraph::QueryVertices() const
	{
		vector<long> res;
		for (auto mapping : _vertID_innerID)
		{
			res.push_back(mapping.first);
		}

		return res;
	}

	vector<long> DirectedGraph::QueryAdjacentVertices(const long id) const
	{
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai;
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai_end;
		//Get index map.
		auto index = boost::get(boost::vertex_index, *_graph);

		auto innerID = GetInnerID(id);

		vector<long> res;
		for (boost::tie(ai, ai_end) = boost::adjacent_vertices(innerID, *_graph);
			ai != ai_end; ++ai)
		{
			auto adjID = index[*ai];//It is inner ID.
			auto vertID = GetVertexID(adjID);
			res.push_back(vertID);
		}

		return res;
	}

	vector<shared_ptr<IVertex>> DirectedGraph::GetAllVertices() const
	{
		auto allIDs = QueryVertices();
		vector<shared_ptr<IVertex>> res;
		for (auto id : allIDs)
		{
			res.push_back(GetVertextProperty(id).vert);
		}

		return res;
	}

	long DirectedGraph::GetInnerID(const shared_ptr<IVertex> vert) const
	{
		return GetInnerID(vert->GetID());
	}

	long DirectedGraph::GetInnerID(const long vertID) const
	{
		return _vertID_innerID.at(vertID);
	}

	long DirectedGraph::CreateNewInnerID(const shared_ptr<IVertex> vert)
	{
		auto newID = _maxInnerID;
		++_maxInnerID;
		//Check if the Vertex ID exists.
		if (_vertID_innerID.find(vert->GetID()) != _vertID_innerID.end())
		{
			throw invalid_argument("The id already exists: " + vert->GetID());
		}
		_vertID_innerID[vert->GetID()] = newID;

		return newID;
	}

	long DirectedGraph::TryGetInnerID(const shared_ptr<IVertex> vert)
	{
		if (_vertID_innerID.find(vert->GetID()) != _vertID_innerID.end())
		{
			return GetInnerID(vert);
		}
		else
		{
			return CreateNewInnerID(vert);
		}
	}

	long DirectedGraph::GetVertexID(const long innerID) const
	{
		for (auto mapping : _vertID_innerID)
		{
			if (mapping.second == innerID)
			{
				return mapping.first;
			}
		}

		throw invalid_argument("Cannot find ID in mapping: " + innerID);
	}

	IVertex::IVertex()
	{

	}

	IVertex::~IVertex()
	{

	}

	

}

