#pragma once
#include "InOut.h"

#include "../MindInterface/CommonFunction.h"

namespace DataCollection
{
	class Word;
	class GrammarPattern;
	enum PartOfSpeech;
}

namespace NeuralNetwork
{
	class iDataArray;
}

namespace Mind
{
	class ConceptSet;
	class iConceptInteractTable;

	struct Edge_Info//��ʾ���ļ���ȡ��ConceptEdge����Ϣ
	{
		Identity to;
		shared_ptr<iConceptInteractTable> modifications;
	};
	struct Connection_Info
	{
		Identity me;
		vector<Edge_Info> edge_infos;
	};

	struct Sen_Gra
	{
		//////////////////////////////////////////////////////////////////////////
		///Vector of Part of Speech.
		//////////////////////////////////////////////////////////////////////////
		vector<int> gra;
	};
	

	namespace CommonFunction
	{
		Identity TransformToIdentity(const string idStr,const string wordStr);

		shared_ptr<NeuralNetwork::iDataArray> ToDataArray(const shared_ptr<iConceptChain> chain, const ConceptSet* conceptSet);
		
		vector<Sen_Gra> InputGraSamples(string file) ;

		//////////////////////////////////////////////////////////////////////////
		//Get sentences with tagging from file.
		//File format should be like this:
		//	������ͬ��ž��ӵı����У�
		//	��� / 0 �� / 0 �� / 7 ͬ / 7 ��ž��� / 0 �� / 9 ���� / 1 �� / 0 �� / 14
		//One normal sentence and one tagged sentence.
		//////////////////////////////////////////////////////////////////////////
		vector<vector<shared_ptr<DataCollection::Word>>> ParseSampleSentences(const string samplePath);

		//////////////////////////////////////////////////////////////////////////
		//Parse a tagged sentence string to Word list.
		//It is used by <ParseSampleSentences>.
		//////////////////////////////////////////////////////////////////////////
		vector<shared_ptr<DataCollection::Word>> ParsePOSTagging(const string line);
	}
}


