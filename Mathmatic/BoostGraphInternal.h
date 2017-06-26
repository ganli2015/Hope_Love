#pragma once
#include "InOut.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/depth_first_search.hpp>

namespace Math
{
	namespace internal
	{
		/*-------------------------Visitors-------------------------*/

		//Detector.
		struct cycle_detector : public boost::dfs_visitor<>
		{
			typedef dfs_visitor<> Base;

			cycle_detector(bool& has_cycle)
				: _has_cycle(has_cycle) { }

			template <class Edge, class Graph>
			void back_edge(Edge e, Graph& g) {
				_has_cycle = true;
				Base::back_edge(e, g);
			}

// 			template <class Vertex, class Graph>
// 			void discover_vertex(Vertex u, Graph g)
// 			{
// 				assert("");
// 				Base::discover_vertex(u, g);
// 			}

			template <class Edge, class Graph>
			void examine_edge(Edge e, Graph& g) {
				assert("");
				Base::examine_edge(e, g);
			}

		protected:
			bool& _has_cycle;
		};

		//Collect all edges.
		class edge_collector : public boost::dfs_visitor<>
		{
			multimap<long, long> _edges;

		public:
			template <class Edge, class Graph>
			void finish_edge(Edge e, Graph&)
			{
				_edges.insert(make_pair(e.from, e.to));
			}

			multimap<long, long> GetEdges() const { return _edges; }
		};

		/*---------------------------------------------------------*/

		//////////////////////////////////////////////////////////////////////////
		//Get default Color map for dfs search...
		//////////////////////////////////////////////////////////////////////////
		template<class Graph>
		boost::iterator_property_map<
			int*,
			typename boost::property_map<Graph, boost::vertex_index_t>::type>
		GetDefaultColorMap(const Graph& graph)
		{
			typedef boost::property_map<Graph, boost::vertex_index_t>::type vertex_index_map;
			typedef boost::iterator_property_map<int*, vertex_index_map> ColorMap;

			//Construct color map.
			std::vector<int> colors_vec(boost::num_vertices(graph));
			ColorMap color(&colors_vec[0],
				get(boost::vertex_index, graph));

			return color;
		}

	}
}