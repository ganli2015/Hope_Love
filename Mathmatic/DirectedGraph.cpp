#include "stdafx.h"
#include "DirectedGraph.h"
#include "BoostGraphInternal.h"

#include "../CommonTools/assertions.h"

namespace Math
{

	DirectedGraph::DirectedGraph(const size_t vertNum)
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
		boost::add_edge(fromID, toID, EdgeProperty(fromID, toID), *_graph);
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
		return (*_graph)[id];
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
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai;
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai_end;
		for (boost::tie(ai, ai_end) = boost::adjacent_vertices(vert->GetID(), *_graph);
			ai != ai_end; ++ai)
			std::cout << (*_graph)[*ai].vert->GetID() << " ";

		auto color = internal::GetDefaultColorMap<GraphImp>(*_graph);
		
		//Search from vert.
		bool has_cycle = false;
		internal::cycle_detector vis(has_cycle);
		boost::depth_first_search(*_graph, vis, color, vert->GetID());

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
		res.reserve(boost::num_vertices(*_graph));

		typedef boost::graph_traits<GraphImp>::vertex_descriptor Vertex;

		// get the property map for vertex indices
		typedef boost::property_map<GraphImp, boost::vertex_index_t>::type IndexMap;
		IndexMap index = boost::get(boost::vertex_index, *_graph);

		typedef boost::graph_traits<GraphImp>::vertex_iterator vertex_iter;
		std::pair<vertex_iter, vertex_iter> vp;
		for (vp = boost::vertices(*_graph); vp.first != vp.second; ++vp.first) {
			Vertex v = *vp.first;
			res.push_back(v);
		}

		return res;
	}

	vector<long> DirectedGraph::QueryAdjacentVertices(const long id) const
	{
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai;
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai_end;
		//Get index map.
		auto index = boost::get(boost::vertex_index, *_graph);

		vector<long> res;
		for (boost::tie(ai, ai_end) = boost::adjacent_vertices(id, *_graph);
			ai != ai_end; ++ai)
		{
			auto adjID = index[*ai];
			res.push_back(adjID);
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

	IVertex::IVertex()
	{

	}

	IVertex::~IVertex()
	{

	}

	

}

