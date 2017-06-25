#include "stdafx.h"
#include "DirectedGraph.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "../CommonTools/assertions.h"

namespace Math
{
	namespace internal
	{
		//Detector.
		struct cycle_detector : public boost::dfs_visitor<>
		{
			cycle_detector(bool& has_cycle)
				: _has_cycle(has_cycle) { }

			template <class Edge, class Graph>
			void back_edge(Edge, Graph&) {
				_has_cycle = true;
			}
		protected:
			bool& _has_cycle;
		};
	}

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
		assert(from->GetID() != to->GetID());
		boost::add_edge(from->GetID(), to->GetID(), *_graph);
		//Set IVertex to property.
		GetVertextProperty(from->GetID()).vert = from;
		GetVertextProperty(to->GetID()).vert = to;
	}

	vector<shared_ptr<IVertex>> DirectedGraph::GetAdjacentVertices(const long id) const
	{
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai;
		typename boost::graph_traits<GraphImp>::adjacency_iterator ai_end;
		//Get index map.
		auto index = boost::get(boost::vertex_index, *_graph);

		vector<shared_ptr<IVertex>> res;
		for (boost::tie(ai, ai_end) = boost::adjacent_vertices(id, *_graph);
			ai != ai_end; ++ai)
		{
			auto adjID = index[*ai];
			res.push_back(GetVertextProperty(adjID).vert);
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

	IVertex::IVertex()
	{

	}

	IVertex::~IVertex()
	{

	}

}

