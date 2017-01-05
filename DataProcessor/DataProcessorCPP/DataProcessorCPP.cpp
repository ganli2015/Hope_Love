// DataProcessorCPP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CommonTools.h"
#include "GrammarSampleGenerator.h"
#include "GrammarPatternExtractor.h"
#include "GrammarLocalDistributionExtractor.h"
#include "ExtractChainFromSentence.h"

#include "TestSegmentation.h"
#include "TestPOSTagging.h"

//#define GENERATE_GRAMMA_SAMPLES
//#define GENERATE_GRAMMA_DISTRIBUTION
//#define EXTRACT_GRAMMARLOCAL_DISTRIBUTION
//#define TEST_SEGMENTATION
//#define TEST_POSTAGGING
#define EXTRACT_CHAIN
int main()
{
#ifdef GENERATE_GRAMMA_SAMPLES
	GrammarSampleGenerator gsg;
	gsg.Run();
#endif // GENERATE_GRAMMA_SAMPLES
	
#ifdef GENERATE_GRAMMA_DISTRIBUTION
	GrammarPatternExtractor gpe;
	gpe.Run("../OutData/str_sample_corpus_de.txt");
#endif // GENERATE_GRAMMA_DISTRIBUTION

#ifdef EXTRACT_GRAMMARLOCAL_DISTRIBUTION
	GrammarLocalDistributionExtractor glde;
	glde.Run("../OutData/str_sample_corpus_de.txt");

#endif // EXTRACT_GRAMMARLOCAL_DISTRIBUTION

#ifdef TEST_SEGMENTATION
	TestSegmentation ts;
	ts.Run("corpus_wo.txt");
#endif

#ifdef TEST_POSTAGGING
	TestPOSTagging tpt;
	tpt.Run(newdir + "corpus_de_pos.txt");
#endif // TEST_POSTAGGING

#ifdef EXTRACT_CHAIN
	ExtractChainFromSentence ecfs;
	ecfs.Run("");
#endif

	logfile.close();

	system("pause");

    return 0;
}

