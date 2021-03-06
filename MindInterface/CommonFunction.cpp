#include "StdAfx.h"
#include "CommonFunction.h"

#include "iConceptInteractTable.h"
#include "iCerebrum.h"
#include "iConceptInteractTable.h"
#include "iConceptChain.h"
#include "iMindElementCreator.h"
#include "PublicTypedef.h"

#include "../DataCollection/GrammaPattern.h"
#include "../DataCollection/Word.h"

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

using namespace DataCollection;

namespace Mind
{
	namespace CommonFunction
	{

		void AppendToInteractTable( const vector<shared_ptr<iConcept>>& from,const vector<shared_ptr<iConcept>>& to,shared_ptr<iConceptInteractTable> table )
		{
			if(table==NULL) return;

			for (size_t i=0;i<from.size();++i)
			{
				for (size_t j=0;j<to.size();++j)
				{
					table->Add(from[i],to[j]);
				}
			}
		}

		void _MINDINTERFACEINOUT AppendToInteractTable(const vector<MindType::ConceptPair>& pairs, shared_ptr<iConceptInteractTable> table)
		{
			for (size_t j = 0; j < pairs.size(); ++j)
			{
				table->Add(pairs[j].first, pairs[j].second);
			}
		}

		int IndexOf(const vector<shared_ptr<iConcept>>& concepts, const shared_ptr<iConcept> concept)
		{
			class IsSameWith
			{
				shared_ptr<iConcept> _val;
			public:
				IsSameWith(const shared_ptr<iConcept> val):_val(val){};
				~IsSameWith(){}

				bool operator()(const shared_ptr<iConcept> val)
				{
					if(CommonFunction::IsSameConcept(_val,val))
					{
						return true;
					}
					else
						return false;
				}
			};

			vector<shared_ptr<iConcept>>::const_iterator iter=find_if(concepts.begin(),concepts.end(),IsSameWith(concept));
			if(iter==concepts.end())
			{
				return -1;
			}
			else
			{
				return distance(concepts.begin(),iter);
			}
		}

		int IndexOf( const map<int,shared_ptr<iConcept>>& concepts,const shared_ptr<iConcept> concept )
		{
			for (map<int,shared_ptr<iConcept>>::const_iterator it=concepts.begin();it!=concepts.end();++it)
			{
				if(it->second->Same(concept))
				{
					return it->first;
				}
			}

			return -1;
		}

		int IndexOf( const map<int,weak_ptr<iConcept>>& concepts,const shared_ptr<iConcept> concept )
		{
			for (map<int,weak_ptr<iConcept>>::const_iterator it=concepts.begin();it!=concepts.end();++it)
			{
				if(it->second.lock()->Same(concept))
				{
					return it->first;
				}
			}

			return -1;
		}


		bool IsSameConcept( const shared_ptr<iConcept> left,const shared_ptr<iConcept> right )
		{
			if(left->GetString()==right->GetString() && left->GetId()==right->GetId())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void WriteConcepts(const vector<shared_ptr<iConcept>>& vec,ofstream& out )
		{
			for (size_t j=0;j<vec.size();++j)
			{
				out<<vec[j]->GetString()<<" ";
			}
			out<<endl;
		}

		void _MINDINTERFACEINOUT LogConcepts(const vector<shared_ptr<iConcept>>& vec)
		{
			for (size_t j = 0; j < vec.size(); ++j)
			{
				DEBUGLOG(vec[j]);
			}
		}

		bool SameConcept::operator()(const shared_ptr<iConcept> val)
		{
			if(val->Same(_me))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void RemoveSameChain( vector<shared_ptr<iConceptChain>>& chains ) 
		{
			vector<shared_ptr<iConceptChain>> removedChains;

			for (size_t i=0;i<chains.size();++i)
			{
				shared_ptr<iConceptChain> curChain=chains[i];

				bool duplicated=false;
				for (size_t j=i+1;j<chains.size();++j)
				{
					if(chains[j]->Same(curChain))
					{
						duplicated=true;
					}
				}

				if(!duplicated)
				{
					removedChains.push_back(curChain);
				}
			}

			chains=removedChains;
		}

		void OutputConceptPairs( const vector<MindType::ConceptPair>& pairs,ostream& out )
		{
			for (size_t i=0;i<pairs.size();++i)
			{
				out<<pairs[i].first->GetString();
				out<<" ";
				out<<pairs[i].second->GetString();
				out<<endl;
			}
		}


		bool SameConceptPair::operator()( const pair<shared_ptr<iConcept>,shared_ptr<iConcept>> val )
		{
			if(val.first->Same(_from) && val.second->Same(_to))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		vector<MindType::ConceptPair> _MINDINTERFACEINOUT FilterPartialConceptPairs( const vector<MindType::ConceptPair>& total, const vector<MindType::ConceptPair>& partial )
		{
			vector<MindType::ConceptPair> res(total);

			for (size_t i=0;i<partial.size();++i)
			{
				vector<MindType::ConceptPair>::iterator samePairIter=find_if(res.begin(),res.end(),CommonFunction::SameConceptPair(partial[i].first,partial[i].second));
				assert(samePairIter!=res.end());

				res.erase(samePairIter);
			}

			return res;
		}

		void RemoveDuplicated(list<shared_ptr<iConceptInteractTable>>& tables)
		{
			vector<shared_ptr<iConceptInteractTable>> copy(tables.begin(), tables.end());
			for (vector<shared_ptr<iConceptInteractTable>>::iterator it= copy.begin();it!= copy.end();++it)
			{
				//Search the tables behind <it>.
				//If there is the same one ,then remove it.
				//Finally the last one will exist.
				if (find_if(it + 1, copy.end(), SameConceptTable(*it)) != copy.end())
				{
					it = copy.erase(it);
				}
			}

			tables = list<shared_ptr<iConceptInteractTable>>(copy.begin(), copy.end());
		}

		shared_ptr<Mind::iConcept> FindIntegerConcept(const vector<shared_ptr<Mind::iConcept>>& conceptVec)
		{
			static string integerStr="����";

			for (size_t i=0;i<conceptVec.size();++i)
			{
				vector<shared_ptr<Mind::iConcept>> forward=conceptVec[i]->GetForwardConcepts();
				//Find a concept of <integerStr> and imply conceptVec[i] is an integer.
				for (size_t j=0;j<forward.size();++j)
				{
					if(forward[j]->GetString()==integerStr)
					{
						return conceptVec[i];
					}
				}
			}

			return NULL;
		}

		bool SameConceptPair_Identity::operator()(const pair<shared_ptr<iConcept>, shared_ptr<iConcept>> val)
		{
			if (val.first->GetIdentity()==_from && val.second->GetIdentity()==_to)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool SameConceptTable::operator()(const shared_ptr<iConceptInteractTable> val)
		{
			if(val->Same(_me))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	}
}




