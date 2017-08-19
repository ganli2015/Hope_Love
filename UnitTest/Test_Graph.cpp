#include "stdafx.h"
#include "Test_Graph.h"

#include <gmock/gmock.h>

#include "../Mathmatic/DirectedGraph.h"
#include "../Mathmatic/TraverseData.h"

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

	TEST_F(Test_Graph, BFS_HasConnection_DisconnectedGraph)
	{
		//Start vertex is v4.

		auto startVert = _v4;
		auto bfsData = _diconnectedGraph.BFS(startVert);
		
		ASSERT_TRUE(bfsData.HasConnection(_v5));
		ASSERT_TRUE(bfsData.HasConnection(_v6));
		ASSERT_FALSE(bfsData.HasConnection(_v7));
		ASSERT_FALSE(bfsData.HasConnection(_v8));
	}

	TEST_F(Test_Graph, BFS_GetPath_DisconnectedGraph)
	{
		//Start vertex is v4.

		auto startVert = _v4;
		auto bfsData = _diconnectedGraph.BFS(startVert);

		//Get path to v5.
		auto pathToV5 = bfsData.GetPathTo(_v5);
		ASSERT_EQ(2, pathToV5.size());
		ASSERT_EQ(5, pathToV5.back()->GetID());

		//Get path to v6.
		auto pathToV6 = bfsData.GetPathTo(_v6);
		ASSERT_EQ(3, pathToV6.size());
		ASSERT_EQ(6, pathToV6.back()->GetID());

		//Get path to v7.No path.
		auto pathToV7 = bfsData.GetPathTo(_v7);
		ASSERT_EQ(0, pathToV7.size());

		//Get path to v8.No path.
		auto pathToV8 = bfsData.GetPathTo(_v8);
		ASSERT_EQ(0, pathToV8.size());
	}

	TEST_F(Test_Graph, BFS_HasConnection_CycleGraph)
	{
		//Start vertex is v0.

		auto startVert = _v0;
		auto bfsData = _graphWithCycle.BFS(startVert);

		ASSERT_TRUE(bfsData.HasConnection(_v1));
		ASSERT_TRUE(bfsData.HasConnection(_v2));
		ASSERT_FALSE(bfsData.HasConnection(_v3));
	}

	TEST_F(Test_Graph, BFS_GetPath_CycleGraph)
	{
		//Start vertex is v3.

		auto startVert = _v3;
		auto bfsData = _graphWithCycle.BFS(startVert);

		//Get path to v5.
		auto pathToV0 = bfsData.GetPathTo(_v0);
		ASSERT_EQ(3, pathToV0.size());
		ASSERT_EQ(0, pathToV0.back()->GetID());
	}

	TEST_F(Test_Graph, BFS_HasCycle)
	{
		//Start vertex is v0.
		//Cycle : v0->v1->v2->v0.

		auto startVert = _v0;
		auto bfsData = _graphWithCycle.BFS(startVert);

		list<shared_ptr<IVertex>> cycle;
		ASSERT_TRUE(bfsData.HasCycle(cycle));

		vector<long> cycleIDs(cycle.size());
		transform(cycle.begin(), cycle.end(), cycleIDs.begin(), 
			[](shared_ptr<IVertex> v)->long {return v->GetID(); });
		ASSERT_THAT(cycleIDs, ElementsAre(0,1,2,0));
	}

	TEST_F(Test_Graph, BFS_HasNoCycle)
	{
		//Start vertex is v3.

		auto startVert = _v3;
		auto bfsData = _graphWithCycle.BFS(startVert);

		list<shared_ptr<IVertex>> cycle;
		ASSERT_FALSE(bfsData.HasCycle(cycle));
	}

	TEST_F(Test_Graph, BFS_HasCycle_GraphWithTwoCycles)
	{
		//Start vertex is v9.

		auto startVert = _v9;
		auto bfsData = _graphWithTwoCycles.BFS(startVert);

		list<shared_ptr<IVertex>> cycle;
		ASSERT_TRUE(bfsData.HasCycle(cycle));

		ASSERT_EQ(4, cycle.size());
	}

	TEST_F(Test_Graph, GenerateAllSubGraphs)
	{
		auto subGraphs = _diconnectedGraph.AllSubGraphs();

		ASSERT_EQ(2, subGraphs.size());
	}
}

