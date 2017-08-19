#include "stdafx.h"
#include "DirectedGraph.h"
#include "BoostGraphInternal.h"
#include "TraverseData.h"

#include "../CommonTools/assertions.h"
#include "../CommonTools/LogWriter.h"

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

	void DirectedGraph::AddVertex(const shared_ptr<IVertex> vert)
	{
		CreateNewInnerID(vert);
		VertexProperty newProp;
		newProp.vert = vert;
		boost::add_vertex(newProp, *_graph);
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
		res->AddVertex(vert);
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
			long vertID;
			if (GetVertexID(adjID, vertID))
			{
				res.push_back(vertID);
			}
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

	bool DirectedGraph::GetVertexID(const long innerID, long& vertID) const
	{
		for (auto mapping : _vertID_innerID)
		{
			if (mapping.second == innerID)
			{
				vertID= mapping.first;
				return true;
			}
		}

		return false;
	}

	Math::TraverseData DirectedGraph::BFS(const shared_ptr<IVertex> startVertex) const
	{
		GraphSearchEngine searchEngine;
		auto result = searchEngine.BFS(this, startVertex->GetID());

		return TraverseData(result,startVertex,this);
	}

	bool DirectedGraph::TryRemoveInnerID(const shared_ptr<IVertex> vert, long& removed)
	{
		if (_vertID_innerID.find(vert->GetID()) != _vertID_innerID.end())
		{
			removed = _vertID_innerID[vert->GetID()];
			_vertID_innerID.erase(vert->GetID());
			return true;
		}
		else
		{
			return false;
		}
	}

	vector<shared_ptr<DirectedGraph>> DirectedGraph::AllSubGraphs()
	{
		vector<shared_ptr<DirectedGraph>> res;
		//Backup _vertID_innerID as it will be modified.
		map<long, long> vertID_innerID_copy = _vertID_innerID;

		while (!vertID_innerID_copy.empty())
		{
			//Get first vertex and generate sub graph.
			auto vert = GetVertextProperty(vertID_innerID_copy.begin()->first).vert;
			auto subGraph = GenerateSubGraph(vert);
			res.push_back(subGraph);

			//Remove data in vertID_innerID.
			auto allVert = subGraph->GetAllVertices();
			for (auto removedVert : allVert)
			{
				vertID_innerID_copy.erase(removedVert->GetID());
			}
		}

		return res;
	}

	IVertex::IVertex()
	{

	}

	IVertex::~IVertex()
	{

	}

	

}

