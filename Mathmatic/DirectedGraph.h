#pragma once
#include "InOut.h"
#include "GraphSearchEngine.h"

#include "../CommonTools/NonCopyable.h"

namespace boost
{
	struct directedS;
	struct vecS;
	struct no_property;
	struct listS;

	template <class OutEdgeListS,
		class VertexListS,
		class DirectedS,
		class VertexProperty,
		class EdgeProperty,
		class GraphProperty,
		class EdgeListS>
		class adjacency_list;
}


namespace Math
{
	//////////////////////////////////////////////////////////////////////////
	//Interface for vertex of a graph.
	//////////////////////////////////////////////////////////////////////////
	class _MATHMATICINOUT IVertex
	{
	public:
		IVertex();;
		~IVertex();;

		virtual long GetID() const = 0;
	};

	class _MATHMATICINOUT DirectedGraph : 
		public IGraphQuerier,
		public CommonTool::NonCopyable
	{
	private:
		struct VertexProperty
		{
			shared_ptr<IVertex> vert;
		};
		struct EdgeProperty
		{
			long from;
			long to;

			EdgeProperty(){}
			EdgeProperty(long from, long to) :from(from), to(to)
			{

			};
		};

		//Use boost for temp.
		typedef boost::adjacency_list<boost::vecS,
			boost::vecS,
			boost::directedS,
			VertexProperty,
			EdgeProperty,
			boost::no_property,
			boost::listS> GraphImp;

		typedef multimap<long, long> EdgeSet;

	private:

		GraphImp *_graph;

	public:
		DirectedGraph(const size_t vertNum);
		~DirectedGraph();

		//////////////////////////////////////////////////////////////////////////
		//Add an edge from <from> to <to>.
		//If these two vertices do not exists, then they will be added.
		//////////////////////////////////////////////////////////////////////////
		void AddEdge(const shared_ptr<IVertex> from, const shared_ptr<IVertex> to);

		//////////////////////////////////////////////////////////////////////////
		//Get "to" vertices that vertex with <id> points to.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<IVertex>> GetAdjacentVertices(const long id) const;

		vector<shared_ptr<IVertex>> GetAllVertices() const;

		bool HasCycle() const;

		//////////////////////////////////////////////////////////////////////////
		//Check if <vert> is in a cycle.
		//////////////////////////////////////////////////////////////////////////
		bool HasCycle(const shared_ptr<IVertex> vert) const;

		//////////////////////////////////////////////////////////////////////////
		//Generate sub graph containing <vert>.
		//The all vertices in the returned graph is connected to <vert>.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<DirectedGraph> GenerateSubGraph(const shared_ptr<IVertex> vert) const;
	private:

		VertexProperty& GetVertextProperty(const long id) const;

		//////////////////////////////////////////////////////////////////////////
		//Get all edges connecting directly or indirectly to vertex with <id>.
		//////////////////////////////////////////////////////////////////////////
		DirectedGraph::EdgeSet GetAllConnectedEdges(const long id) const;

		//////////////////////////////////////////////////////////////////////////
		//Get all vertices IDs.
		//////////////////////////////////////////////////////////////////////////
		virtual vector<long> QueryVertices() const;
		virtual vector<long> QueryAdjacentVertices(const long id) const;
	};

}

