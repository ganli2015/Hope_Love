#pragma once
#include "InOut.h"

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

	//////////////////////////////////////////////////////////////////////////
	//Find all values with key in the multimap <myMap>.
	//////////////////////////////////////////////////////////////////////////
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