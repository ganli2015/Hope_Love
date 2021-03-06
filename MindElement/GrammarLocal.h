#pragma once
#include "InOut.h"
#include "../Mathmatic/MyInt.h"

#include "../DataCollection/Word.h"

namespace Mind
{
	//////////////////////////////////////////////////////////////////////////
	///GrammarLocal contains information of surrounding POSes of a POS.
	//////////////////////////////////////////////////////////////////////////
	class _MINDELEMENTINOUT GrammarLocal
	{
		//////////////////////////////////////////////////////////////////////////
		///POS of <me>
		//////////////////////////////////////////////////////////////////////////
		DataCollection::PartOfSpeech _pos;
		//////////////////////////////////////////////////////////////////////////
		///The distribution of frequencies of POS behind <me>.
		//////////////////////////////////////////////////////////////////////////
		map<DataCollection::PartOfSpeech,Math::MyInt> _forward;
		//////////////////////////////////////////////////////////////////////////
		///The distribution of frequencies of POS in front of <me>.
		//////////////////////////////////////////////////////////////////////////
		map<DataCollection::PartOfSpeech,Math::MyInt> _backward;

	public:
		GrammarLocal();
		GrammarLocal(const DataCollection::PartOfSpeech myPos);
		~GrammarLocal();

		//////////////////////////////////////////////////////////////////////////
		///Add the frequency of POS in front of <me> by one.
		//////////////////////////////////////////////////////////////////////////
		void AddForward(const DataCollection::PartOfSpeech& pos);
		//////////////////////////////////////////////////////////////////////////
		///Add the frequency of POS behind <me> by one.
		//////////////////////////////////////////////////////////////////////////
		void AddBackward(const DataCollection::PartOfSpeech& pos);


		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes in the front.
		//////////////////////////////////////////////////////////////////////////
		double GetRatio_Forward(const DataCollection::PartOfSpeech& pos) const;
		//////////////////////////////////////////////////////////////////////////
		///Get the ratio between the frequency of <pos> and total frequency among all POSes behind.
		//////////////////////////////////////////////////////////////////////////
		double GetRatio_Backward(const DataCollection::PartOfSpeech& pos) const;

		//////////////////////////////////////////////////////////////////////////
		///Read data from file.
		//////////////////////////////////////////////////////////////////////////
		void Read(ifstream& in);
	private:
		Math::MyInt TotalCount(const map<DataCollection::PartOfSpeech,Math::MyInt>& myMap) const;
	};
}


