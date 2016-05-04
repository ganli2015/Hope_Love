#include "StdAfx.h"
#include "SingleExpression.h"

#include "../SentenceAnalysisAlgorithm/SentenceAnalyzer.h"
#include "../SentenceAnalysisAlgorithm/WordRelationTableBuilder.h"

#include "../DataCollection/Sentence.h"

#include "../MindInterface/iConceptInteractTable.h"

using namespace DataCollection;
using namespace Mind;

namespace LogicSystem
{
	SingleExpression::SingleExpression(void)
	{
	}


	SingleExpression::SingleExpression( const string val )
	{
		SentenceAnalyzer sentenceAnalyzer(val);
		sentenceAnalyzer.Analyze();
		_sen=sentenceAnalyzer.GetAnalyzedSentences();
	}

	SingleExpression::SingleExpression(const shared_ptr<DataCollection::Sentence> val)
	{
		if(!val->StructureAnalyzed())
		{
			SentenceAnalyzer sentenceAnalyzer(val->GetString());
			sentenceAnalyzer.Analyze();
			_sen=sentenceAnalyzer.GetAnalyzedSentences();
		}
		else
		{
			_sen=val;
		}
	}

	SingleExpression::~SingleExpression(void)
	{
	}

	shared_ptr<Mind::iConceptInteractTable> SingleExpression::GetBaseInteractTable() const
	{
		if(_sen==NULL) return NULL;
		if(!_sen->StructureAnalyzed()) return NULL;

		WordRelationTableBuilder builder(_sen);
		builder.Build();
		return builder.GetBaseInteractTable();
	}

	shared_ptr<Mind::iConceptInteractTable> SingleExpression::GetProtoInteractTable() const
	{
		if(_sen==NULL) return NULL;
		if(!_sen->StructureAnalyzed()) return NULL;

		WordRelationTableBuilder builder(_sen);
		builder.Build();
		return builder.GetProtoInteractTable();
	}

}

