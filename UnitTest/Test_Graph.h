#pragma once
#include "PublicHeader.h"

#include "../Mathmatic/DirectedGraph.h"

namespace Math
{
	class SimpleVertex : public IVertex
	{
		long _id;
	public:
		SimpleVertex(const long id) :_id(id) {};
		~SimpleVertex() {};

		virtual long GetID() const { return _id; };
	};

	class Test_Graph : public testing::Test
	{
	protected:
		//0->1,1->2,2->0,2->3
		static DirectedGraph _graphWithCycle;
		static shared_ptr<SimpleVertex> _v0;
		static shared_ptr<SimpleVertex> _v1;
		static shared_ptr<SimpleVertex> _v2;
		static shared_ptr<SimpleVertex> _v3;

	public:
		Test_Graph();
		~Test_Graph();

		static void SetUpTestCase() 
		{
			//Prepare graph.
			_v0 = make_shared<SimpleVertex>(0);
			_v1 = make_shared<SimpleVertex>(1);
			_v2 = make_shared<SimpleVertex>(2);
			_v3 = make_shared<SimpleVertex>(3);

			_graphWithCycle.AddEdge(_v0, _v1);
			_graphWithCycle.AddEdge(_v1, _v2);
			_graphWithCycle.AddEdge(_v2, _v0);

			_graphWithCycle.AddEdge(_v2, _v3);
		}
	private:

	};

	Math::DirectedGraph Test_Graph::_graphWithCycle(4);

	shared_ptr<SimpleVertex> Test_Graph::_v0;

	shared_ptr<SimpleVertex> Test_Graph::_v1;

	shared_ptr<SimpleVertex> Test_Graph::_v2;

	shared_ptr<SimpleVertex> Test_Graph::_v3;

	Test_Graph::Test_Graph()
	{
		
	}

	Test_Graph::~Test_Graph()
	{
	}
}
