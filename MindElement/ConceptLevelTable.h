#pragma once
#include "InOut.h"
#include "../MindInterface/iConceptLevelTable.h"

namespace Mind
{
	class iConcept;
	class BaseConcept;

	//��ʾĳ��Concept����BaseConcept֮��ľ���
	class _MINDELEMENTINOUT ConceptLevelTable : public iConceptLevelTable
	{
	private:
		struct LevelInfo
		{
			shared_ptr<iConcept> base;
			int level;
		};

		shared_ptr<iConcept> _myConcept;
		vector<LevelInfo> _levels;
	public:
		ConceptLevelTable(const shared_ptr<iConcept> val);
		virtual ~ConceptLevelTable(void);

		//Add a distance <level> to a base concept <concept>.
		//<concept> should be a base concept.
		//If there already exists <concept> in me, then update the level to <concept> with the smaller one.
		//Closer to base concept, larger the level is.
		void Insert(const shared_ptr<iConcept> concept, const int level);
		//<me>��concept֮���level�������ͬһ��concept������0.�������û�����ӹ�ϵ������-1.
		int LevelTo(const shared_ptr<iConcept> concept) const;
	private:
		int FindLevelInfoIndex(const shared_ptr<iConcept> concept) const;
	};
}


