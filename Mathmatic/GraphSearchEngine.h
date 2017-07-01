#pragma once
#include "InOut.h"
#include <queue>

#include "../CommonTools/CommonStringFunction.h"

namespace Math
{
	class IVertex;

	//////////////////////////////////////////////////////////////////////////
	//Interface for querying information of Graph.
	//////////////////////////////////////////////////////////////////////////
	class IGraphQuerier
	{

	public:
		//////////////////////////////////////////////////////////////////////////
		//Get "to" vertices's ids that vertex with <id> points to.
		//////////////////////////////////////////////////////////////////////////
		virtual vector<long> QueryAdjacentVertices(const long id) const = 0;

		//////////////////////////////////////////////////////////////////////////
		//Get all vertices of <me>.
		//////////////////////////////////////////////////////////////////////////
		virtual vector<long> QueryVertices() const =0;
	};

	//////////////////////////////////////////////////////////////////////////
	//Result data structure for search.
	//The ID in <me> is the Vertex ID.
	//This class is for Internal use.
	//////////////////////////////////////////////////////////////////////////
	class SearchResult
	{
		typedef multimap<long, long> EdgeSet;
		typedef map<long, double> DistanceMap;

		EdgeSet _edges;
		DistanceMap _distanceMap;

		//////////////////////////////////////////////////////////////////////////
		//Key is vertex id and value is its parent id.
		//////////////////////////////////////////////////////////////////////////
		map<long, long> _parentMap;

	public:
		SearchResult() {};
		~SearchResult() {};

		multimap<long, long> GetEdges() const { return _edges; }
		map<long, double> GetDistanceMap() const{ return _distanceMap; }
		//////////////////////////////////////////////////////////////////////////
		//Return a parent-child relation map.
		//Key is vertex id and value is its parent id.
		//////////////////////////////////////////////////////////////////////////
		map<long, long> GetParentMap() const { return _parentMap; }
		double GetDistance(const long id) const { return _distanceMap.at(id); }

		void AddEdge(const long from, const long to) { _edges.insert(make_pair(from, to)); }
		void SetDistance(const long id, const double distance) { _distanceMap[id] = distance; }
		//////////////////////////////////////////////////////////////////////////
		//Set parent id for vertex <id>.
		//Note! Each vertex has only one parent which is the vertex searching for him!
		//////////////////////////////////////////////////////////////////////////
		void SetParent(const long id, const long parentID)
		{
			_parentMap[id] = parentID;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//Search algorithm for graph.
	//////////////////////////////////////////////////////////////////////////
	class GraphSearchEngine
	{
		enum Color
		{
			Black,
			White,
			Gray
		};

		typedef map<long, Color> ColorMap;

		queue<long> _vertexQueue;
		ColorMap _colorMap;

	public:
		GraphSearchEngine();
		~GraphSearchEngine();

		//////////////////////////////////////////////////////////////////////////
		//BFS from the start ID.
		//And search all reachable vertices.
		//////////////////////////////////////////////////////////////////////////
		SearchResult BFS(const IGraphQuerier* graph,const long startID);

	private:
	};
}



