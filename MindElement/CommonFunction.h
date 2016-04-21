#pragma once
#include "InOut.h"

namespace Mind
{
	struct Identity
	{
		string str;
		//表示同一个string所对应的不同的concept。引导词的id等于-1！
		//目前同一个单词的同一个词性只对应于一个id！
		//也就是说这个id暂时起不到作用！
		int id;
	};

	class Concept;
	class ConceptInteractTable;
	class ConceptChain;
	namespace CommonFunction
	{
		//把<from>和<to>的所有相互作用都添加到table
		void AppendToInteractTable(const vector<shared_ptr<Concept>>& from,const vector<shared_ptr<Concept>>& to,shared_ptr<ConceptInteractTable> table);
	
		//如果没有找到，则返回-1.
		int _MINDELEMENTINOUT IndexOf(const vector<shared_ptr<Concept>>& concepts,const shared_ptr<Concept> concept);

		bool IsSameConcept(const shared_ptr<Concept> left,const shared_ptr<Concept> right);

		void _MINDELEMENTINOUT WriteConcepts(const vector<shared_ptr<Concept>>& vec,ofstream& out);

		class _MINDELEMENTINOUT SameConcept
		{
			shared_ptr<Concept> _me;
		public:
			SameConcept(const shared_ptr<Concept> val):_me(val){}
			~SameConcept(){}

			bool operator()(const shared_ptr<Concept> val);
		};
	}
}


