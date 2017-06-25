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


