#include "stdafx.h"
#include "Test_Graph.h"

#include "../Mathmatic/DirectedGraph.h"

namespace Math
{
	TEST(Test_Graph, PointToTwoVertices)
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

	TEST(Test_Graph, PointToOneVerticesAndPointedByAnotherOne)
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

	TEST(Test_Graph, HasCycleWithThreeVertices)
	{
		//0,,1,2 is a cycle and 3 is not in the cycle.

		DirectedGraph graph(3);
		auto v0 = make_shared<SimpleVertex>(0);
		auto v1 = make_shared<SimpleVertex>(1);
		auto v2 = make_shared<SimpleVertex>(2);
		auto v3 = make_shared<SimpleVertex>(3);

		graph.AddEdge(v0, v1);
		graph.AddEdge(v1, v2);
		graph.AddEdge(v2, v0);

		graph.AddEdge(v2, v3);

		ASSERT_TRUE(graph.HasCycle());
	}

	TEST(Test_Graph, HasNoCycle)
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
}

