#pragma once
#include "InOut.h"
#include <set>

namespace CommonTool
{
	void _COMMONTOOLSINOUT SleepForSeconds(const int seconds);

	template<class T>
	void TryDeletePointer(T *ptr)
	{
		if (ptr != NULL)
		{
			delete ptr;
			ptr = NULL;
		}
	}

	//Find all values with key in the multimap <myMap>.
	template<class KeyType,class ValueType>
	vector<ValueType> FindAll(const KeyType& key,const multimap<KeyType, ValueType>& myMap)
	{
		multimap<KeyType, ValueType>::const_iterator beg = myMap.lower_bound(key);
		multimap<KeyType, ValueType>::const_iterator end = myMap.upper_bound(key);

		vector<ValueType> res;
		while (beg != end)
		{
			res.push_back(beg->second);
			beg++;
		}

		return res;
	}

	//Find same elements in two vectors.
	template<class T>
	vector<T> FindSameInVectors(const vector<T>& v1, const vector<T>& v2)
	{
		set<T> v2Set(v2.begin(), v2.end());

		vector<T> res;
		for (auto elem1 : v1)
		{
			if (v2Set.find(elem1) != v2Set.end())
			{
				res.push_back(elem1);
			}
		}

		return res;
	}
}

template<class Key, class Value>
ostream& operator<<(ostream& out, const pair<Key, Value>& p)
{
	out << p.first << " " << p.second;
	return out;
}

template<class T>
ostream& operator<<(ostream& out, const vector<T>& vec)
{
	out << "{";
	size_t outSize = vec.size() > 10 ? 10 : vec.size();
	for (size_t i = 0; i < outSize; ++i)
	{
		out << vec[i];
		if (i != outSize - 1)
		{
			out << " ";
		}
	}

	out << "}";
	return out;
}