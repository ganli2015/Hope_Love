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
}
