#include "stdafx.h"
#include "GrammarPatternModel.h"
#include "GrammaSet.h"

#include "../Mathmatic/FindSequence.h"

#include "../CommonTools/LogWriter.h"

#include "../DataCollection/GrammaPattern.h"
#include "../DataCollection/LanguageFunc.h"

using namespace DataCollection;
using namespace Math;

namespace Mind
{
	GrammarPatternModel::GrammarPatternModel(GrammarSet* val):_grammarSet(val)
	{
		_totalFreq = _grammarSet->GetTotalFrequency();
	}


	GrammarPatternModel::~GrammarPatternModel()
	{
	}

	double GrammarPatternModel::ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		//Possibility equals the sum of ratio of each grammar pattern.
		//Therefore, possibility must be in the interval of 0 to 1.

		GrammarPattern pattern = LanguageFunc::ConvertToPattern(sentence);

		double sumFreq = 0.;
		vector<GrammarPattern> matchedPattern = _grammarSet->ContainSubsequence(pattern);
		for (size_t j = 0; j < matchedPattern.size(); ++j)
		{
			MyInt curFreq(_grammarSet->GetFreqencyofPattern(matchedPattern[j]));
			sumFreq += curFreq / _totalFreq;
		}

		return sumFreq;
	}

	void GrammarPatternModel::ExtractGrammarPatternFromInitialFile(const string filename) const
	{
		GrammarPatternExtractor extractor;
		extractor.Run(filename);
	}

	GrammarPatternExtractor::GrammarPatternExtractor()
	{
	}


	GrammarPatternExtractor::~GrammarPatternExtractor()
	{
	}


	void GrammarPatternExtractor::Run(const string file) const
	{
		ExtractGrammarPatternFromInitialFile(file);
	}

	void GrammarPatternExtractor::OutputPatternDistribution(const unordered_map<string, int>& p_d) const
	{
		ofstream out("GrammaPatterns_Initial2.txt");
		out.clear();
		for (unordered_map<string, int>::const_iterator it = p_d.begin(); it != p_d.end(); ++it)
		{
			out << it->first << it->second << endl;
		}

		out.close();

	}

	void GrammarPatternExtractor::ExtractGrammarPatternFromInitialFile(const string file) const
	{
		vector<Sen_Gra> samples = InputGraSamples(file);

		// 	std::vector<std::vector<int>> allcommon_seqs = FindAllCommonSequences(samples);
		// 
		// 	vector<Pattern_Distribution> p_d = ComputePatternDistribution(allcommon_seqs);
		// 
		// 	OutputPatternDistribution(p_d);

		unordered_map<string, int> allcommon_seqs = FindAllCommonSequencesAndDistribution(samples);

		OutputPatternDistribution(allcommon_seqs);
	}

	unordered_map<string, int> GrammarPatternExtractor::FindAllCommonSequencesAndDistribution(const vector<Sen_Gra>& samples) const
	{
		//Each sequence only interact with those whose length is not much longer or shorter then its length.
		//And the region is determined by <lengthOffset>.
		//It is in consideration of dealing with large samples.
		int lengthOffset = 0;

		unordered_map<string, int> res;

		for (unsigned int i = 0; i < samples.size(); ++i)
		{
			vector<int> gra1 = samples[i].gra;
			for (unsigned int j = i + 1; j < samples.size(); ++j)
			{
				if (abs((int)samples[j].gra.size() - (int)samples[i].gra.size()) > lengthOffset) continue;

				try
				{
					vector<int> gra2 = samples[j].gra;
					std::vector<std::vector<int>> common_seqs;
					Math::FindAllCommonSubsequence(gra1, gra2, common_seqs);
					//Convert common sequences to strings, and make them keys in a map.
					vector<string> seqStrings = ConvertToStrings(common_seqs);

					for (unsigned int k = 0; k < seqStrings.size(); ++k)
					{
						if (res.find(seqStrings[k]) == res.end())
						{
							//if not find the common sequence, create one.
							res[seqStrings[k]] = 1;
						}
						else
						{
							res[seqStrings[k]]++;
						}
					}
				}
				catch (const std::exception& e)
				{
					LOG_FORMAT2("Error in FindAllCommonSubsequence. List index1: %d. List index2: %d.", i, j);
					LOG_EXCEPTION(e);
				}

			}

			cout << i << endl;
		}

		return res;
	}

	vector<string> GrammarPatternExtractor::ConvertToStrings(const vector<vector<int>>& commonSeq) const
	{
		vector<string> res;
		for (unsigned int i = 0; i < commonSeq.size(); ++i)
		{
			string seqStr = "";
			vector<int> oneSeq = commonSeq[i];
			for (unsigned int j = 0; j < oneSeq.size(); ++j)
			{
				seqStr += to_string(oneSeq[j]) + " ";
			}
			res.push_back(seqStr);
		}

		return res;
	}

	vector<GrammarPatternExtractor::Sen_Gra> GrammarPatternExtractor::InputGraSamples(const string file)
	{
		vector<Sen_Gra> samples;

		ifstream in(file);
		if (!in) return samples;

		while (!in.eof())
		{
			Sen_Gra sample;

			int count;
			in >> count;
			vector<int> gra;
			gra.reserve(count);
			for (int i = 1; i <= count; ++i)
			{
				int elem;
				in >> elem;
				gra.push_back(elem);
			}

			sample.gra = gra;
			samples.push_back(sample);
		}

		return samples;
	}
}

