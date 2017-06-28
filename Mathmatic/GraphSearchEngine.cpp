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
					_colorMap[adjVert] = Gray;
					double dis = res.GetDistance(curVert) + 1;
					res.SetDistance(adjVert, dis);
					_vertexQueue.push(adjVert);
				}
			}
			_colorMap[curVert] = Black;
		}

		return res;
	}
}

