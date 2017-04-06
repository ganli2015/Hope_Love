#include "StdAfx.h"
#include "CommonFunction.h"
#include "../MindElement/Concept.h"
#include "../MindElement/ConceptInteractTable.h"
#include "ConceptSet.h"
#include "../MindElement/ConceptChain.h"
#include "Cerebrum.h"
#include "../MindElement/BaseConcept.h"

#include <fstream>
#include <sstream>

#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/GrammaPattern.h"

#include "../Neural Network Design/DataArray.h"

using namespace DataCollection;
using namespace Math;
using namespace NeuralNetwork;

namespace Mind
{
	namespace CommonFunction
	{
		Mind::Identity TransformToIdentity( const string idStr,const string wordStr )
		{
			stringstream ss(idStr);//原来string与其他类型转换

			int id_me;
			ss>>id_me;
// 			if(!ss.good())
// 			{
// 				throw runtime_error("Error in TransformToWordID");
// 			}
			Identity me;
			me.id=id_me;
			me.str=wordStr;

			return me;
		}

		shared_ptr<NeuralNetwork::iDataArray> ToDataArray( const shared_ptr<iConceptChain> chain , const ConceptSet* conceptSet)
		{
			//初始化，所有元素等于0
			shared_ptr<iDataArray> res(new DataArray(conceptSet->BaseConceptCount()));

			vector<shared_ptr<iConcept>> conceptSequence=chain->GetConceptVec();
			for (size_t i=0;i<conceptSequence.size();++i)
			{
				shared_ptr<BaseConcept> base=dynamic_pointer_cast<BaseConcept>(conceptSequence[i]);
				if(base==NULL)
				{
					LOG_DESC("Some concept is not a base concept in this chain.", chain);
					throw runtime_error("Error in ToDataArray");
				}

				res->Set_ithVal(base->GetBaseId(),i+1);
			}

			return res;
		}

		vector<Sen_Gra> InputGraSamples(string file) 
		{
			vector<Sen_Gra> samples;

			ifstream in(file);
			if (!in) return samples;

			while (!in.eof())
			{
				Sen_Gra sample;

				int count;
				in >> count;
				//			Vector array(NUM_PARTOFSPEECH);
				vector<int> gra;
				gra.reserve(count);
				for (int i = 1; i <= count; ++i)
				{
					int elem;
					in >> elem;
					//				++array[elem];
					gra.push_back(elem);
				}

				sample.gra = gra;
				//			array.Normalize();
				samples.push_back(sample);
			}

			return samples;
		}
	}
}
