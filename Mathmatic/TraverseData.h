#pragma once
#include "InOut.h"
#include "GraphSearchEngine.h"

#include <list>

namespace Math
{
	class IVertex;
	class DirectedGraph;

	//////////////////////////////////////////////////////////////////////////
	//The result data of BFS search or DFS search of Graph.
	//The search is start from a certain vertex.
	//////////////////////////////////////////////////////////////////////////
	class _MATHMATICINOUT TraverseData
	{
		typedef multimap<long, long> EdgeSet;
		typedef map<long, double> DistanceMap;

		EdgeSet _edges;
		DistanceMap _distanceMap;
		//////////////////////////////////////////////////////////////////////////
		//Key is vertex id and value is its parent id.
		//////////////////////////////////////////////////////////////////////////
		map<long, long> _parentMap;

		shared_ptr<IVertex> _startVert;
		//////////////////////////////////////////////////////////////////////////
		//All vertex of the graph.
		//////////////////////////////////////////////////////////////////////////
		map<long, shared_ptr<IVertex>> _vertexMap;

		friend class DirectedGraph;

	public:
		~TraverseData();

		//////////////////////////////////////////////////////////////////////////
		//Check if the start vertex is connected to vertex <to>.
		//////////////////////////////////////////////////////////////////////////
		bool HasConnection(const shared_ptr<IVertex> to) const;

		//////////////////////////////////////////////////////////////////////////
		//The count of vertices that start vertex has connection to.
		//////////////////////////////////////////////////////////////////////////
		int AdjacentCount() const { return _distanceMap.size(); }

		//////////////////////////////////////////////////////////////////////////
		//Get the shortest path to vertex <to>.
		//The head is the start vertex.
		//////////////////////////////////////////////////////////////////////////
		list<shared_ptr<IVertex>> GetPathTo(const shared_ptr<IVertex> to) const;

		//////////////////////////////////////////////////////////////////////////
		//Check if the start vertex is in a cycle.
		//If true, then output the cycle in which head and tail are both the start vertex.
		//////////////////////////////////////////////////////////////////////////
		bool HasCycle(list<shared_ptr<IVertex>>& cycle) const;

		shared_ptr<DirectedGraph> GenerateGraph() const;

		//////////////////////////////////////////////////////////////////////////
		//Get all vertices in me except the start vertex.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<IVertex>> GetAllConnectVertice() const;
	private:

		TraverseData(const SearchResult res, const shared_ptr<IVertex> vert, const DirectedGraph* graph);

	};
}

