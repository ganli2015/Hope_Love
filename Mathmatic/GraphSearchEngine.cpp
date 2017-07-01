#include "stdafx.h"
#include "GraphSearchEngine.h"


namespace Math
{
	GraphSearchEngine::GraphSearchEngine()
	{
	}


	GraphSearchEngine::~GraphSearchEngine()
	{
	}

	Math::SearchResult GraphSearchEngine::BFS(const IGraphQuerier* graph, const long startID)
	{
		//Refer to Introduction to Algorithm for detail.

		SearchResult res;

		//Prepare color map.
		auto allVert = graph->QueryVertices();
		for (auto vertID : allVert)
		{
			_colorMap[vertID] = White;
		}
		//Handle the first vertex.
		_colorMap[startID] = Gray;
		res.SetDistance(startID, 0.0);
		_vertexQueue.push(startID);

		//Search other vertex.
		while (!_vertexQueue.empty())
		{
			auto curVert = _vertexQueue.front();
			_vertexQueue.pop();
			for (auto adjVert : graph->QueryAdjacentVertices(curVert))
			{
				//Push edge to result.
				res.AddEdge(curVert, adjVert);

				if (_colorMap[adjVert] == White)
				{
					//It is reached for the first time.
					_colorMap[adjVert] = Gray;
					double dis = res.GetDistance(curVert) + 1;
					res.SetDistance(adjVert, dis);
					res.SetParent(adjVert, curVert);
					_vertexQueue.push(adjVert);
				}

				//Check if searched vertex is the start vertex.
				//If so, it indicates that the start vertex is in a cycle, otherwise it cannot be searched.
				if (adjVert == startID)
				{
					//Then set the parent of the start vertex.
					//Therefore the cycle can be tracked.
					res.SetParent(startID, curVert);
				}
			}
			_colorMap[curVert] = Black;
		}

		return res;
	}
}

