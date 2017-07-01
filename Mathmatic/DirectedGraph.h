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
	class TraverseData;

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

	//////////////////////////////////////////////////////////////////////////
	//A directed graph.
	//In some member functions, the input parameter "id" refers to the ID of IVertex.
	//////////////////////////////////////////////////////////////////////////
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
		//Mapping from IVertex's ID to GraphImp's ID.
		//There is mapping from IVertex's ID to graph's ID.
		//The reason is that graph's ID will automatically increase to the max of IVertex's ID,
		//and some default vertices is created by graph which have no relation with IVertex.
		//Therefore, I make graph's ID increased one by one and create a mapping between both IDs.
		//The input parameter 'id' in almost member functions means IVertex's ID and it may be converted
		//in the function to inner ID to communicate with graph.
		map<long, long> _vertID_innerID;
		//Max inner ID. It will increase by one if a new vertex is added.
		long _maxInnerID;
	public:
		DirectedGraph(const size_t vertNum);
		~DirectedGraph();

		TraverseData BFS(const shared_ptr<IVertex> startVertex) const;

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
		//Generate sub graph containing <vert>.
		//The all vertices in the returned graph is connected to <vert>.
		//////////////////////////////////////////////////////////////////////////
		shared_ptr<DirectedGraph> GenerateSubGraph(const shared_ptr<IVertex> vert) const;
	private:
		//////////////////////////////////////////////////////////////////////////
		//Get Inner ID from IVertex's ID.
		//////////////////////////////////////////////////////////////////////////
		long GetInnerID(const shared_ptr<IVertex> vert) const;
		long GetInnerID(const long vertID) const;
		//////////////////////////////////////////////////////////////////////////
		//Create a new inner ID and it will be added to the map.
		//////////////////////////////////////////////////////////////////////////
		long CreateNewInnerID(const shared_ptr<IVertex> vert); 
		//////////////////////////////////////////////////////////////////////////
		//Try to Get Inner ID from IVertex's ID.
		//If have not found the Inner ID, then create one.
		//////////////////////////////////////////////////////////////////////////
		long TryGetInnerID(const shared_ptr<IVertex> vert);
		//////////////////////////////////////////////////////////////////////////
		//Get IVertex's ID from Inner ID.
		//////////////////////////////////////////////////////////////////////////
		long GetVertexID(const long innerID) const;

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

