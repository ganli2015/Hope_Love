#pragma once
#include "InOut.h"
#include "PublicTypedef.h"

namespace Mind
{
	class iConcept;

	

	//Concept�໥���õ��б������໥������ConceptEdge��ͬ�������Ǵ�����ConceptSet���ԭ�͵����ӣ�
	//��һ���Ѿ��н��ι̹�ϵ�����ӡ�ǰ��������ʱ�����ӣ�ͨ�����������µ�֪ʶ�µ�����ʱ�����ġ�
	class _MINDINTERFACEINOUT iConceptInteractTable
	{
		typedef MindType::ConceptPair ConceptPair;


	public:
		iConceptInteractTable();
		virtual ~iConceptInteractTable(void);

		virtual void Add(const shared_ptr<iConcept> from, const shared_ptr<iConcept> to) =0;
		//�õ���ʩ��������Concept
		virtual vector<shared_ptr<iConcept>> GetFromConcept(const shared_ptr<iConcept> concept) const =0;
		//�õ�������������Concept
		virtual vector<shared_ptr<iConcept>> GetToConcept(const shared_ptr<iConcept> concept) const =0;
		virtual vector<ConceptPair> GetAllRelations() const =0;

		//�ϲ�ConceptInteractTable
		virtual void Absorb(const shared_ptr<iConceptInteractTable> absorbed) =0;
		virtual void InteractDeeper() =0;
	};
}

