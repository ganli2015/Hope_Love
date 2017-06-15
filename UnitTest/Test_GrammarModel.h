#pragma once
#include "PublicHeader.h"

namespace Mind
{
#ifndef _DEBUG
	class GrammarFeatureModel;
	
	class Test_GrammarFeatureModel :public testing::Test
	{
	protected:
		static GrammarFeatureModel* _featureModel;

		struct Sample
		{
			vector<string> sentences;
			string expectOptimal;
		};
		static vector<Sample> _samples;


		static void SetUpTestCase();
		static void TearDownTestCase();

		static string ComputeOptimal(const Sample& sample);

	private: 
		static void PrepareSamples();
	};
#endif // !DEBUG
}


