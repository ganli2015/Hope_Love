#include "StdAfx.h"
#include "RelationLeaf.h"

#include "../MindInterface/iSymbol.h"
#include "../MindInterface/iExpression.h"
#include "../MindInterface/iConceptInteractTable.h"
#include "../MindInterface/iConcept.h"
#include "../MindInterface/iRelationConstraint.h"

#include "../Mathmatic/FindSequence.h"

using namespace Mind;
namespace LogicSystem
{
	RelationLeaf::RelationLeaf(void):_connectTag("->"),_sepTag(',')
	{
	}


	RelationLeaf::~RelationLeaf(void)
	{
	}

	void RelationLeaf::AddRelation( const shared_ptr<ConSymbol> from,const shared_ptr<ConSymbol> to )
	{
		assert(from!=NULL && to!=NULL);
		_relations.push_back(make_pair(from,to));
	}

	void RelationLeaf::AddConstraint( const shared_ptr<iRelationConstraint> constraint )
	{
		assert(constraint!=NULL);
		_constraints.push_back(constraint);
	}

	std::string RelationLeaf::GetString() const
	{
		string res="";
		for (unsigned int i=0;i<_relations.size();++i)
		{
			res+=_relations[i].first->GetString();
			res+=_connectTag;
			res+=_relations[i].second->GetString();
			
			if(i!=_relations.size()-1)
			{
				res+=_sepTag;
			}
		}

		return res;
	}

	bool RelationLeaf::Satisfy( const shared_ptr<iExpression> expre ) const
	{
		shared_ptr<iConceptInteractTable> interTable=expre->GetProtoInteractTable();
		
		return InterTableSatisfyRelation(interTable);
	}

	bool RelationLeaf::InterTableSatisfyRelation( const shared_ptr<iConceptInteractTable> interTable ) const
	{
		vector<vector<PairInfo>> matchedPairSeq=FindMatchedPairSequence(interTable->GetAllRelations());

		for (unsigned int i=0;i<matchedPairSeq.size();++i)
		{
			if(SatifyConstraint(matchedPairSeq[i],_constraints))//Any of them satisfies ,then we consider <expre> satisfying.
			{
				return true;
			}
		}

		return false;
	}

	vector<iRelation::PairSequence> RelationLeaf::FindMatchedPairSequence(const vector<ConceptPair>& conceptPairs) const
	{
		vector<vector<PairInfo>> matchedPairSeq=FindMatchedPairSequence(_relations,conceptPairs);

		return matchedPairSeq;
	}

	vector<RelationLeaf::ConceptPair> RelationLeaf::FindMatchedPairs(const SymbolPair& symbolPair,const vector<ConceptPair>& cPairs)
	{
		class FindConceptPair
		{
			SymbolPair _symbolPair;
			vector<ConceptPair> _result;

		public:
			FindConceptPair(const SymbolPair& val):_symbolPair(val){}
			~FindConceptPair(){}
			
			vector<ConceptPair> GetResult() const {return _result;}
			void operator()(const ConceptPair& cPair)
			{
				if(_symbolPair.first->Match(cPair.first) && _symbolPair.second->Match(cPair.second))
				{
					_result.push_back(cPair);
				}
			}
		};

		FindConceptPair findPairs(symbolPair);
		findPairs=for_each(cPairs.begin(),cPairs.end(),findPairs);
		vector<RelationLeaf::ConceptPair> res=findPairs.GetResult();

		return res;
	}

	vector<vector<RelationLeaf::PairInfo>> RelationLeaf::FindMatchedPairSequence(const vector<SymbolPair>& sPairs,const vector<ConceptPair>& cPairs)
	{
		class FindEachRelatedConceptPairs
		{
			vector<ConceptPair> _cPairs;

		public:
			FindEachRelatedConceptPairs(const vector<ConceptPair>& cPairs):_cPairs(cPairs){}
			~FindEachRelatedConceptPairs(){}

			vector<PairInfo> operator()(const SymbolPair& sPair)
			{
				vector<PairInfo> res;
				vector<ConceptPair> relatedPairs= FindMatchedPairs(sPair,_cPairs);
				res.reserve(relatedPairs.size());
				for (unsigned int i=0;i<relatedPairs.size();++i)
				{
					PairInfo info;
					info.cPair=relatedPairs[i];
					info.sPair=sPair;
					res.push_back(info);
				}

				return res;
			}
		};

		//Each element in <relatedPairs> is all the concept pairs that one symbol pair is related with.
		vector<vector<PairInfo>> relatedPairs;
		transform(sPairs.begin(),sPairs.end(),back_inserter(relatedPairs),FindEachRelatedConceptPairs(cPairs));

		//Each element in <pairSequence> is concept pairs that all different symbol pairs are related with.
		vector<vector<PairInfo>> pairSequence=Math::GetAllCombinations<PairInfo>::Get(relatedPairs);

		return pairSequence;
	}

}
