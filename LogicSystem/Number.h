#pragma once
#include "InOut.h"
#include "../MindInterface/iSymbol.h"
#include "../MindInterface/iConcept.h"
#include "../CommonTools/CommonStringFunction.h"

namespace LogicSystem
{
	///A Number represents an integer in a logic system.
	template<class T>
	class Number : public iSymbol<T>
	{
		static int NumNum;
		static string TypeName;

		int _id;

	public:
		Number(void){ ++NumNum; };
		~Number(void){};

		static shared_ptr<Number> Create();

		virtual bool Match(const shared_ptr<T> con) const ;
		virtual string GetString() const;
		virtual void BindReferredObject(const shared_ptr<T> obj) ;
	};

	///Specialize for iConcept
	template<>
	bool _LOGICSYSTEMINOUT Number<Mind::iConcept>::Match( const shared_ptr<Mind::iConcept> con ) const;

	template<class T>
	int Number<T>::NumNum=0;

	template<class T>
	string Number<T>::TypeName=typeid(T).name();

	template<class T>
	shared_ptr<Number<T>> Number<T>::Create()
	{
		return shared_ptr<Number>(new Number());
	}

// 	template<class T>
// 	bool Number<T>::Match( const shared_ptr<T> con ) const
// 	{
// 		if(typeid(Mind::iConcept).name()==TypeName)
// 		{
// 			//Check whether the forward concepts of <con> exists ���� concept.
// 			vector<shared_ptr<Mind::iConcept>> forward=con->GetBase();
// 			for (unsigned int i=0;i<forward.size();++i)
// 			{
// 				if(forward[i]->GetString()=="����")
// 				{
// 					return true;
// 				}
// 			}
// 
// 			return false;
// 		}
// 		else
// 			throw runtime_error("Not implemented!!");
// 	}

	template<class T>
	void Number<T>::BindReferredObject(const shared_ptr<T> obj) 
	{
		if(obj==NULL)
		{
			_referredObj=NULL;
			return;
		}

		if(Match(obj))
		{
			_referredObj=obj;
		}
	}

	template<class T>
	string LogicSystem::Number<T>::GetString() const
	{
		return "Num"+CommonTool::ToString(_id);
	}

}

