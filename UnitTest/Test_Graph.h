#pragma once
#include "PublicHeader.h"

#include "../Mathmatic/DirectedGraph.h"

namespace Math
{
	class SimpleVertex : public IVertex
	{
		static long VertexCount;

		long _id;
	public:
		SimpleVertex() :_id(VertexCount++) {};
		SimpleVertex(const long id) :_id(id) {};
		~SimpleVertex() {};

		virtual long GetID() const { return _id; };
	};
	long SimpleVertex::VertexCount = 0;;


	class Test_Graph : public testing::Test
	{
	protected:
		//0->1,1->2,2->0,3->2
		//			3
		//			|
		//			2
		//		   / \
		//		 0-----1
		static DirectedGraph _graphWithCycle;
		static shared_ptr<SimpleVertex> _v0;
		static shared_ptr<SimpleVertex> _v1;
		static shared_ptr<SimpleVertex> _v2;
		static shared_ptr<SimpleVertex> _v3;

		//4->5,5->6,6->4,7->8
		//		  7--8
		//			
		//			6
		//		   / \
		//		 4-----5
		static DirectedGraph _diconnectedGraph;
		static shared_ptr<SimpleVertex> _v4;
		static shared_ptr<SimpleVertex> _v5;
		static shared_ptr<SimpleVertex> _v6;
		static shared_ptr<SimpleVertex> _v7;
		static shared_ptr<SimpleVertex> _v8;

		//        10    12
		//       /  \  /  \
		//     11 --  9 -- 13
		static DirectedGraph _graphWithTwoCycles;
		static shared_ptr<SimpleVertex> _v9;
		static shared_ptr<SimpleVertex> _v10;
		static shared_ptr<SimpleVertex> _v11;
		static shared_ptr<SimpleVertex> _v12;
		static shared_ptr<SimpleVertex> _v13;

	public:
		Test_Graph();
		~Test_Graph();

		static void SetUpTestCase() 
		{
			//The vertex id should be the same as index of vi.

			//Prepare graph with cycle.
			_graphWithCycle.AddEdge(_v0, _v1);
			_graphWithCycle.AddEdge(_v1, _v2);
			_graphWithCycle.AddEdge(_v2, _v0);
			_graphWithCycle.AddEdge(_v3, _v2);

			//Prepare disconnected graph.
			_diconnectedGraph.AddEdge(_v4, _v5);
			_diconnectedGraph.AddEdge(_v5, _v6);
			_diconnectedGraph.AddEdge(_v6, _v4);
			_diconnectedGraph.AddEdge(_v7, _v8);

			//Prepare graph with two cycles.
			_graphWithTwoCycles.AddEdge(_v9, _v10);
			_graphWithTwoCycles.AddEdge(_v10, _v11);
			_graphWithTwoCycles.AddEdge(_v11, _v9);
			_graphWithTwoCycles.AddEdge(_v9, _v12);
			_graphWithTwoCycles.AddEdge(_v12, _v13);
			_graphWithTwoCycles.AddEdge(_v13, _v9);
		}
	private:

	};

	Math::DirectedGraph Test_Graph::_graphWithCycle(4);

	Math::DirectedGraph Test_Graph::_diconnectedGraph(5);

	Math::DirectedGraph Test_Graph::_graphWithTwoCycles(5);

	shared_ptr<SimpleVertex> Test_Graph::_v0 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v1 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v2 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v3 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v4 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v5 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v6 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v7 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v8 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v9 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v10 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v11 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v12 = make_shared<SimpleVertex>();
	shared_ptr<SimpleVertex> Test_Graph::_v13 = make_shared<SimpleVertex>();

	Test_Graph::Test_Graph()
	{
		
	}

	Test_Graph::~Test_Graph()
	{
	}
}
