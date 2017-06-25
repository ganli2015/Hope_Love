#pragma once
#include "InOut.h"

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


	class _MATHMATICINOUT DirectedGraph : public CommonTool::NonCopyable
	{
	private:
		struct VertexProperty
		{
			shared_ptr<IVertex> vert;
		};

		//Use boost for temp.
		typedef boost::adjacency_list<boost::vecS,
			boost::vecS,
			boost::directedS,
			VertexProperty,
			boost::no_property,
			boost::no_property,
			boost::listS> GraphImp;

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

		bool HasCycle() const;

	private:

		VertexProperty& GetVertextProperty(const long id) const;
	};

}

