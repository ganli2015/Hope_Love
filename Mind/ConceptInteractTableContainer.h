#pragma once
#include "InOut.h"

namespace Mind
{
	class ConceptInteractTable;
	class iConcept;

	//存放ConceptInteractTable的类，作为对Concept连接的临时记忆体
	class ConceptInteractTableContainer : public MyObject
	{
	private:
		struct TableInfo  
		{
			shared_ptr<ConceptInteractTable> table;
			pair<shared_ptr<iConcept>,shared_ptr<iConcept>> protoPair;//<table>所对应的两个相互作用的concept
		};

		vector<TableInfo> _tableInfos;
	public:
		ConceptInteractTableContainer(void);
		~ConceptInteractTableContainer(void);

		void Add(const shared_ptr<ConceptInteractTable> table,const shared_ptr<iConcept> from, const shared_ptr<iConcept> to);
		vector<pair<shared_ptr<iConcept>,shared_ptr<iConcept>>> GetAllInteractPairs() const;
	};

}

