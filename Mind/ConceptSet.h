#pragma once
#include "InOut.h"
#include "CommonFunction.h"
#include "MindObject.h"
#include "../MindElement/Concept.h"
#include "../MindElement/BaseConcept.h"

namespace DataCollection
{
	class Character;
	class Word;
	enum PartOfSpeech;
}

namespace Mind
{
	class ConceptDatabase;
	class iConceptInteractTable;
	struct DescMatchedConceptInfo;

	class _MINDINOUT ConceptSet : public Obj<ConceptSet> , public MindObject
	{
		typedef std::vector<shared_ptr<Concept>> ConceptMap;
		typedef std::multimap<std::string,shared_ptr<BaseConcept>> BaseConceptMap;
		//ConceptMap _conceptset;
		//BaseConceptMap _baseConceptset;
		shared_ptr<ConceptDatabase> _conceptDB;

		typedef ConceptMap::iterator conceptIter;
		typedef ConceptMap::const_iterator const_conceptIter;
		typedef BaseConceptMap::iterator baseConceptIter;
		typedef BaseConceptMap::const_iterator const_baseConceptIter;
	public:
		ConceptSet(void);
		virtual ~ConceptSet(void);

	    bool IsConceptExist(const std::string str) const;
		
		void AddConcept(const shared_ptr<DataCollection::Word> word);
		
		void AddConcept(const shared_ptr<DataCollection::Word> word,int id);
		void AddBaseConcept(const shared_ptr<DataCollection::Word> word,int id);
		
		void MakeConnection(const shared_ptr<DataCollection::Word> from,const shared_ptr<DataCollection::Word> to);
		
		//Add modification between <from> and <to>.
		void AddModification(const Identity& from,const Identity& to,const shared_ptr<iConceptInteractTable>& modification);
		
		//Get the concept of <word>.The returned concept is copy of the one in <me>.
		shared_ptr<iConcept> GetConceptPtr(const shared_ptr<DataCollection::Word> word) const ;
		shared_ptr<iConcept> GetConceptPtr(const Identity identity) const ;
		shared_ptr<BaseConcept> GetBaseConcept(const int id) const;
		shared_ptr<DataCollection::Word> SearchWord(const Identity identity);

		vector<shared_ptr<DataCollection::Word>> GetAllWordsOfPOS(const DataCollection::PartOfSpeech pos) const;

		int MaxLength_WordWithHead(const shared_ptr<DataCollection::Character> headChara) const;

		std::vector<shared_ptr<DataCollection::Word>> GetAllKindsofWord(const shared_ptr<DataCollection::Word> word) const;

		vector<shared_ptr<iConcept>> SearchForwardConcepts(const shared_ptr<iConcept> concept) const;
		vector<shared_ptr<iConcept>> SearchBackwardConcepts(const shared_ptr<iConcept> concept) const;

		shared_ptr<iConceptInteractTable> GetInteractTable(const shared_ptr<iConcept> from,const shared_ptr<iConcept> to,double level=0);
		shared_ptr<iConceptInteractTable> GetDeepInteractTable(const shared_ptr<iConcept> from,const shared_ptr<iConcept> to);

		int BaseConceptCount() const;

		vector<shared_ptr<iConcept>> GetAllBaseConcepts() const ;

		vector<shared_ptr<iConcept>> GetAllConcepts() const ;

		vector<shared_ptr<iConcept>> GetConcepts(const string str) const ;

		vector<shared_ptr<iConcept>> FindConceptWithMatchedDisc(const shared_ptr<iConceptInteractTable> description) const ;		
		void FindConceptWithMatchedDisc(const shared_ptr<iConceptInteractTable> description, vector<DescMatchedConceptInfo>& matchedInfos) const ;	

		//////////////////////////////////////////////////////////////////////////
		//Collect new base concepts from the sample file <filePath>.
		//The file should be POS tagged.
		//////////////////////////////////////////////////////////////////////////
		void CollectNewBaseConcepts(const string filePath);

	private:
		void Initialize();

		//���˽�г�Ա������
		shared_ptr<Concept> GetConceptRef(const shared_ptr<DataCollection::Word> word) const ;
		shared_ptr<Concept> GetConceptRef(const Identity identity) const ;
		shared_ptr<Concept> GetConceptRef(const shared_ptr<iConcept> concept) const ;

		Identity GetIdentity(const Concept& concept) const;
		void CheckWordIDExist(const shared_ptr<DataCollection::Word> word,const int id);
		void CheckBaseWordIDExist(const shared_ptr<DataCollection::Word> word, const int id,const BaseConceptMap& conceptset );

	};

	class ConceptCollector
	{
		ConceptDatabase *_conceptDB;
	public:
		ConceptCollector(ConceptDatabase *db) :_conceptDB(db) {};
		~ConceptCollector() {};

		//////////////////////////////////////////////////////////////////////////
		//Collect new base concepts from the sample file <filePath>.
		//The file should be POS tagged.
		//////////////////////////////////////////////////////////////////////////
		void Collect(const string filePath);

	private:
	};
}


