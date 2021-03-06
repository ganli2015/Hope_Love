#pragma once
#pragma warning(disable:4251)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#include "InOut.h"

#include <map>
#include <stdlib.h>  
#include <fstream>

#define _RECORD_OBJECTS

///MyObject contains information of total objects.
class _COMMONTOOLSINOUT MyObject
{
protected:
	struct MyObjectInfo
	{
		MyObject* pointer;
		string classname;
	};

	class SamePointer
	{
		void* _p;
	public:
		SamePointer(void* p):_p(p){}
		bool operator()(const MyObjectInfo& info)
		{
			if(info.pointer==_p)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

protected:
	static ofstream out;
	static int count;
	static vector<MyObjectInfo> objectsVec;

public:
	MyObject();
	virtual ~MyObject();

	static int GetObjectCount() {return count;}	
	static string CurrentObjInfo ();
};

///An Object contains type information.
template<class T>
class Obj : public MyObject
{
public:

	void operator delete(void* p);  

	void* operator new(size_t size);
};

template<class T>
void Obj<T>::operator delete( void* p )
{
#ifdef _RECORD_OBJECTS

// 	vector<MyObjectInfo>::iterator it=find_if(objectsVec.begin(),objectsVec.end(),SamePointer(p));
// 	objectsVec.erase(it);

	MyObject::count--;
//	out<<MyObject::count<<" "<<typeid(T).name()<<"-"<<endl;
#endif // _DEBUG

	free(p);
}

template<class T>
void* Obj<T>::operator new( size_t size )
{
	void* p = malloc(size);  

#ifdef _RECORD_OBJECTS
	MyObject::count++;
// 	MyObjectInfo info;
// 	info.pointer=reinterpret_cast<MyObject*>(p);
// 	info.classname=typeid(T).name();
// 
// 	objectsVec.push_back(info);
// 	out<<MyObject::count<<" "<<info.classname<<"+"<<endl;
#endif // _DEBUG

	return p;
}
