#include "stdafx.h"
#include "Test_Graph.h"

#include <gmock/gmock.h>

#include "../Mathmatic/DirectedGraph.h"

using namespace testing;

namespace Math
{
	TEST_F(Test_Graph, PointToTwoVertices)
	{
		//One source vertex, pointing to two vertices.

		DirectedGraph graph(3);
		auto fromV = make_shared<SimpleVertex>(0);
		auto toV1 = make_shared<SimpleVertex>(1);
		auto toV2 = make_shared<SimpleVertex>(2);

		graph.AddEdge(fromV, toV1);
		graph.AddEdge(fromV, toV2);

		auto adjVert = graph.GetAdjacentVertices(0);

		ASSERT_EQ(2, adjVert.size());

		ASSERT_EQ(1, adjVert[0]->GetID());
		ASSERT_EQ(2, adjVert[1]->GetID());
	}

	TEST_F(Test_Graph, PointToOneVerticesAndPointedByAnotherOne)
	{
		//One center vertex, pointing to one vertex and pointed by another one.

		DirectedGraph graph(3);
		auto fromV = make_shared<SimpleVertex>(0);
		auto toV1 = make_shared<SimpleVertex>(1);
		auto toV2 = make_shared<SimpleVertex>(2);

		graph.AddEdge(fromV, toV1);
		graph.AddEdge(toV2, fromV);

		auto adjVert = graph.GetAdjacentVertices(0);

		ASSERT_EQ(1, adjVert.size());

		ASSERT_EQ(1, adjVert[0]->GetID());
	}

	TEST_F(Test_Graph, HasCycleWithThreeVertices)
	{
		//0,,1,2 is a cycle and 3 is not in the cycle.
		ASSERT_TRUE(_graphWithCycle.HasCycle());
	}

	TEST_F(Test_Graph, HasCycleAroundAVertex)
	{
		//Cycle: 0->1->2->0.

		ASSERT_TRUE(_graphWithCycle.HasCycle(_v0));
		ASSERT_TRUE(_graphWithCycle.HasCycle(_v1));
		ASSERT_TRUE(_graphWithCycle.HasCycle(_v2));

		//v3 has no connection to v0,v1,v2.
		ASSERT_FALSE(_graphWithCycle.HasCycle(_v3));
	}

	TEST_F(Test_Graph, HasNoCycle)
	{
		DirectedGraph graph(3);
		auto v0 = make_shared<SimpleVertex>(0);
		auto v1 = make_shared<SimpleVertex>(1);
		auto v2 = make_shared<SimpleVertex>(2);

		graph.AddEdge(v0, v1);
		graph.AddEdge(v1, v2);
		graph.AddEdge(v0, v2);

		ASSERT_FALSE(graph.HasCycle());
	}

	TEST_F(Test_Graph, GenerateSubGraphOfDisconnectedGraph)
	{
		//Generate sub graph containing V4.
		//4->5,5->6,6->4.

		auto subGraphV4 = _diconnectedGraph.GenerateSubGraph(_v4);
		auto subVertV4 = subGraphV4->GetAllVertices();
		ASSERT_EQ(3, subVertV4.size());

		//Check each element.
		vector<long> subVertIDsV4;
		for (auto sub : subVertV4)
		{
			subVertIDsV4.push_back(sub->GetID());
		}
		ASSERT_THAT(subVertIDsV4, ElementsAre(4, 5, 6));
	}

	TEST_F(Test_Graph, GenerateSubGraphOfDisconnectedGraph2)
	{
		//Generate sub graph containing V7.
		//7->8.

		auto subGraphV7 = _diconnectedGraph.GenerateSubGraph(_v7);
		auto subVertV7 = subGraphV7->GetAllVertices();
		ASSERT_EQ(2, subVertV7.size());

		//Check each element.
		vector<long> subVertIDsV7;
		for (auto sub : subVertV7)
		{
			subVertIDsV7.push_back(sub->GetID());
		}
		ASSERT_THAT(subVertIDsV7, ElementsAre(7,8));
	}
}

