#include "StdAfx.h"
#include "ConceptSet.h"
#include "CommonFunction.h"
#include "ConceptSetInitializer.h"
#include "FindConnectConcept.h"
#include "DBContainer.h"
#include "ConceptDatabase.h"
#include <fstream>
#include <deque>
#include <sstream>

#include "../CommonTools/IOFunction.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Character.h"
#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/CommonFunctions.h"

#include "../MindElement/ConceptInteractTable.h"
#include "../MindElement/BaseConcept.h"
#include "../MindElement/MindElementCreator.h"

#include "../MindInterface/iCerebrum.h"

using namespace std;
using namespace DataCollection;

int ToInt(char str)
{
	return (int)str;
}

namespace Mind
{

	ConceptSet::ConceptSet(void):_conceptDB(_dbContainer->GetConceptDatabase())
	{
		Initialize();
		LOG("Initialized ConceptSet.");
	}


	ConceptSet::~ConceptSet(void)
	{
	}

	bool ConceptSet::IsConceptExist( const std::string str ) const
	{
		if (_conceptDB->HasString(str))
			return true;
		else if(LanguageFunc::IsPuncture(shared_ptr<Word>(new Word(str))))
		{
			return true;
		}
		else return false;
	}

	void ConceptSet::CollectNewBaseConcepts(const string filePath)
	{
		auto conceptDB = this->_conceptDB.release();
		ConceptCollector collector(conceptDB);
		collector.Collect(filePath);

		this->_conceptDB.reset(conceptDB);
	}

	int ConceptSet::MaxLength_WordWithHead(const shared_ptr<DataCollection::Character> headChara) const
	{
		//Get all concepts with head <headChara>.
		auto conceptsWithHead = _conceptDB->GetConceptsWithHead(headChara);

		int maxLength = 0;
		for (auto concept : conceptsWithHead)
		{
			auto word = concept->GetWord();
			auto length = word->NumOfChara();
			if (length > maxLength)
			{
				maxLength = length;
			}
		}

		return maxLength;
	}

	void ConceptSet::Initialize()
	{
//		ConceptSetInitializer::InitializeBaseConcept(this,GetHopeLoveMindPath()+BaseConceptsStringFilename);
//		cout << "Base concepts initialized." << endl;

//		ConceptSetInitializer::InitializeNonBaseConcept(this,GetHopeLoveMindPath()+NonBaseConceptString_InitialFilename);
//		cout << "Non-Base concepts initialized." << endl;

//		ConceptSetInitializer::InitializeConceptConnection(this,GetHopeLoveMindPath()+ConceptConnections_InitialFilename);
//		cout << "Concept connection initialized." << endl;


#ifdef _CheckInitialConceptData
		try
		{
			ConceptSetInitializer::CheckInitialConceptData(this);
		}
		catch(...)
		{
			system("pause");
		}
#endif

	}

	std::vector<shared_ptr<DataCollection::Word>> ConceptSet::GetAllKindsofWord(const shared_ptr<DataCollection::Word> word) const
	{
		if (LanguageFunc::IsPuncture(word))//Check if it is a punctuation.
		{
			vector<shared_ptr<DataCollection::Word>> res;
			res.push_back(LanguageFunc::GetParticularWord(word->GetString(), Punctuation));
			return res;
		}

		string str=word->GetString();
		//Get all concepts with word <str>.
		auto concepts = _conceptDB->GetConceptsWithWord(str);
		//Collect words in concepts.
		vector<shared_ptr<Word>> res;
		for (auto concept : concepts)
		{
			res.push_back(concept->GetWord());
		}

		return res;
	}

	vector<shared_ptr<iConcept>> ConceptSet::SearchForwardConcepts( const shared_ptr<iConcept> concept ) const
	{
		shared_ptr<Concept> myConcept=GetConceptRef(concept);
		if(myConcept==NULL)
		{
			throw runtime_error("Error in SearchForwardConcepts");
		}

		FindConnectConcept findConnectConcept;
		return findConnectConcept.FindForward(myConcept);
	}

	vector<shared_ptr<iConcept>> ConceptSet::SearchBackwardConcepts( const shared_ptr<iConcept> concept ) const
	{
		shared_ptr<Concept> myConcept=GetConceptRef(concept);
		if(myConcept==NULL)
		{
			throw runtime_error("Error in SearchForwardConcepts");
		}

		FindConnectConcept findConnectConcept;
		return findConnectConcept.FindBackward(myConcept);
	}

	void ConceptSet::AddConcept(const shared_ptr<DataCollection::Word> word)
	{
		_conceptDB->AddNonBaseConcept(word);
	}

	void ConceptSet::AddConcept(const shared_ptr<DataCollection::Word> word,int id)
	{
		CheckWordIDExist(word,id);			

		_conceptDB->AddNonBaseConcept(word, id);
	}

	void ConceptSet::AddBaseConcept(const shared_ptr<DataCollection::Word> word,int id )
	{
		//Add to _baseConceptset
		//CheckBaseWordIDExist(word,id,_baseConceptset);
		shared_ptr<BaseConcept> newConcept = this->_elemCreator->
			CreateBaseConcept(word, id, _conceptDB->GetBaseConceptCount());
		string str=word->GetString();
		_conceptDB->AddBaseConcept(newConcept);

		//Add to _conceptset
		AddConcept(word, id);
	}

	void ConceptSet::MakeConnection( const shared_ptr<Word> from,const shared_ptr<Word> to )
	{
		shared_ptr<Concept> pfromConcept=GetConceptRef(from);
		shared_ptr<Concept> ptoConcept=GetConceptRef(to);
		if(pfromConcept==NULL || ptoConcept==NULL)
		{
			throw runtime_error("Error in MakeConnection: Cannot find the concept!");
		}

		int fromID = pfromConcept->GetId();
		int toID = ptoConcept->GetId();

		_conceptDB->AddConnection(pfromConcept->GetWord(), fromID, ptoConcept->GetWord(), toID);
	}

	void ConceptSet::AddModification( const Identity& from,const Identity& to,const shared_ptr<iConceptInteractTable>& modification )
	{
		shared_ptr<Concept> pfromConcept=GetConceptRef(from);
		shared_ptr<Concept> ptoConcept=GetConceptRef(to);
		if(pfromConcept==NULL || ptoConcept==NULL )
		{
			throw runtime_error("Error in MakeConnection: Cannot find the concept!");
		}

		int fromID = pfromConcept->GetId();
		int toID = ptoConcept->GetId();

		_conceptDB->AddModification(pfromConcept->GetWord(), fromID, ptoConcept->GetWord(), toID, modification);
	}

	shared_ptr<Concept> ConceptSet::GetConceptRef(const shared_ptr<DataCollection::Word> word) const 
	{
		return _conceptDB->GetConcept(word);
	}

	shared_ptr<Concept> ConceptSet::GetConceptRef( const Identity identity ) const
	{
		return _conceptDB->GetConcept(identity.str, identity.id);
	}

	shared_ptr<Concept> ConceptSet::GetConceptRef(const shared_ptr<iConcept> concept) const 
	{
		Identity identity;
		identity.str=concept->GetString();
		identity.id=concept->GetId();

		return GetConceptRef(identity);
	}

	Mind::Identity ConceptSet::GetIdentity( const Concept& concept ) const
	{
		Identity res;
		res.str=concept.GetString();
		res.id=concept.GetId();
		return res;
	}

	shared_ptr<DataCollection::Word> ConceptSet::SearchWord( Identity identity )
	{
		shared_ptr<iConcept> concept_me=GetConceptPtr(identity);
		if(concept_me==NULL)
		{
			throw runtime_error("Error in BuildConceptConnection");
		}

		shared_ptr<Word> teset= concept_me->GetWord();

		return teset;
	}

	shared_ptr<iConceptInteractTable> ConceptSet::GetInteractTable( const shared_ptr<iConcept> from,const shared_ptr<iConcept> to,double level )
	{
		shared_ptr<iConceptInteractTable> res=from->InteractWith(to);
		
		for (int i=0;i<level;++i)
		{
			res->InteractDeeper();
		}

		return res;
	}

	shared_ptr<iConceptInteractTable> ConceptSet::GetDeepInteractTable( const shared_ptr<iConcept> from,const shared_ptr<iConcept> to )
	{
		shared_ptr<iConceptInteractTable> res=from->DeepInteractWith(to);

		return res;
	}

	int ConceptSet::BaseConceptCount() const
	{
		return _conceptDB->GetBaseConceptCount();
	}

	vector<shared_ptr<DataCollection::Word>> ConceptSet::GetAllWordsOfPOS(const PartOfSpeech pos) const
	{
		vector<shared_ptr<Word>> res;

		auto conceptsWithPOS = _conceptDB->GetConceptsWithPOS(pos);
		for (auto concept : conceptsWithPOS)
		{
			res.push_back(concept->GetWord());
		}

		return res;
	}

	void ConceptSet::CheckWordIDExist(const shared_ptr<DataCollection::Word> word, const int id )
	{
		auto concept = _conceptDB->GetNonBaseConcept(id, word->GetString());
		if (concept != NULL)
		{
			throw runtime_error(CommonTool::StringFormat("Word %s and ID %d already exist.", word->GetString().c_str(), id));
		}
	}

	void ConceptSet::CheckBaseWordIDExist(const shared_ptr<DataCollection::Word> word, const int id,const BaseConceptMap& conceptset )
	{
		auto concept = _conceptDB->GetBaseConcept(id, word->GetString());
		if (concept != NULL)
		{
			throw runtime_error(CommonTool::StringFormat("Word %s and ID %d already exist.", word->GetString().c_str(), id));
		}
	}

	shared_ptr<BaseConcept> ConceptSet::GetBaseConcept( const int id ) const
	{
		return _conceptDB->GetBaseConcept(id);
	}

	shared_ptr<iConcept> ConceptSet::GetConceptPtr( const shared_ptr<DataCollection::Word> word ) const
	{
		shared_ptr<Concept> ref=GetConceptRef(word);
		if(ref==NULL)
		{
			return NULL;
		}
		else
		{
			return ref->Copy();
		}
	}

	shared_ptr<iConcept> ConceptSet::GetConceptPtr( const Identity identity ) const
	{
		shared_ptr<Concept> ref=GetConceptRef(identity);
		if(ref==NULL)
		{
			return NULL;
		}
		else
		{
			return ref->Copy();
		}
	}

	vector<shared_ptr<iConcept>> ConceptSet::FindConceptWithMatchedDisc( const shared_ptr<iConceptInteractTable> description ) const
	{
		class CheckMatch
		{
			shared_ptr<iConceptInteractTable> _description;
		public:
			CheckMatch(const shared_ptr<iConceptInteractTable> description):_description(description){}
			~CheckMatch(){}

			bool operator()(const shared_ptr<Concept>& concept)
			{
				if(concept->MatchWithDescription(_description))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};
		//Get all non base concepts from database.
		auto allNonbaseConcepts = _conceptDB->GetAllNonBaseConcepts();
		
		vector<shared_ptr<iConcept>> res;

		const_conceptIter matchedIter=find_if(allNonbaseConcepts.begin(), allNonbaseConcepts.end(),CheckMatch(description));
		while(matchedIter!= allNonbaseConcepts.end())//Go through all concepts until reaching the end.
		{
			res.push_back(*matchedIter);
			++matchedIter;
			matchedIter=find_if(matchedIter, allNonbaseConcepts.cend(),CheckMatch(description));
		}

		return res;
	}


	void ConceptSet::FindConceptWithMatchedDisc( const shared_ptr<iConceptInteractTable> description, vector<DescMatchedConceptInfo>& matchedInfos ) const
	{
		class GenerateMatchInfo
		{
			shared_ptr<iConceptInteractTable> _description;

			vector<DescMatchedConceptInfo> _infos;
		public:
			GenerateMatchInfo(const shared_ptr<iConceptInteractTable> description):_description(description){}
			~GenerateMatchInfo(){}

			void operator()(const shared_ptr<Concept>& concept)
			{
				shared_ptr<iConcept> toConcept;

				if(concept->MatchWithDescription(_description,toConcept))
				{
					assert(toConcept!=NULL);
					DescMatchedConceptInfo info;
					info.matchedConcept=concept;
					info.toConcept=toConcept;
					_infos.push_back(info);

				}
			}

			vector<DescMatchedConceptInfo> GetResult() const {return _infos; }
		};

		//Get all non base concepts from database.
		auto allNonbaseConcepts = _conceptDB->GetAllNonBaseConcepts();

		matchedInfos.clear();
		
		GenerateMatchInfo genarate(description);
		genarate=for_each(allNonbaseConcepts.begin(), allNonbaseConcepts.end(),genarate);

		matchedInfos=genarate.GetResult();
	}

	void ConceptCollector::Collect(const string filePath)
	{
		auto allPOSsentences = CommonFunction::ParseSampleSentences(filePath);
		LOG_FORMAT("Finish parse sample sentences.The count of sentences is %d.", allPOSsentences.size());

		int newConceptCount = 0;
		for (auto sentence : allPOSsentences)
		{
			for (auto word : sentence)
			{
				//Check the word existence in database.
				if(_conceptDB->HasWord(word)) continue;

				this->_conceptDB->AddBaseConcept(word);
				++newConceptCount;
			}
		}

		LOG_FORMAT("%d new base concepts are added.", newConceptCount);
	}

}

