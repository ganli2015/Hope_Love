#include "stdafx.h"
#include "GrammarLocalModel.h"
#include "CommonFunction.h"

#include "../CommonTools/LogWriter.h"

#include "../DataCollection/Word.h"
#include "../DataCollection/GrammaPattern.h"
#include "../DataCollection/LanguageFunc.h"

#include "../MindElement/GrammarLocal.h"

using namespace DataCollection;

namespace Mind
{
	GrammarLocalModel::GrammarLocalModel()
	{
	}


	GrammarLocalModel::~GrammarLocalModel()
	{
	}

	double GrammarLocalModel::ComputePossiblity(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		GrammarPattern pattern = LanguageFunc::ConvertToPattern(sentence);

		vector<PartOfSpeech> poses = pattern.GetPattern();
		//Remove punctuations as they may interfere computation.
		for (vector<PartOfSpeech>::iterator it = poses.begin(); it != poses.end();)
		{
			if (*it == Punctuation)
			{
				it = poses.erase(it);
			}
			else
			{
				++it;
			}
		}

		if (poses.size() <= 1) return 0.;

		double res = 0.;
		for (size_t i = 0; i < poses.size(); ++i)
		{
			PartOfSpeech curPos = poses[i];
			if (i == 0)//第一个词性只考虑与第二个词性之间的置信度.
			{
				double p_cur_for = GetP_Forward(curPos, poses[i + 1]);
				double p_for_cur = GetP_Backward(poses[i + 1], curPos);
				res += p_for_cur*p_cur_for;
			}
			else if (i == poses.size() - 1)//最后一个词性只考虑与倒数第二个之间的置信度.
			{
				double p_cur_back = GetP_Backward(curPos, poses[i - 1]);
				double p_back_cur = GetP_Forward(poses[i - 1], curPos);
				res += p_back_cur*p_cur_back;
			}
			else
			{
				res += ComputeP_GrammarLocal(curPos, poses[i + 1], poses[i - 1]);
			}
		}

		return res / poses.size();
	}

	void GrammarLocalModel::ExtractGrammarLocalDistribution()
	{
		vector<Sen_Gra> samples = CommonFunction::InputGraSamples(GetHopeLoveMindPath() + StringGrammar_InitialFilename);
		if (samples.empty()) return;

		//Initialize each POS with GrammarLocal.
		map<PartOfSpeech, GrammarLocal> grammarLocalTable;
		for (int i = 0; i < NUM_PARTOFSPEECH; ++i)
		{
			_grammarLocalTable[PartOfSpeech(i)] = shared_ptr<GrammarLocal>(new GrammarLocal(PartOfSpeech(i)));
		}

		//Statistic the distribution of frequencies of POS from <samples>.
		for (size_t i = 0; i < samples.size(); ++i)
		{
			vector<int> gra = samples[i].gra;
			for (size_t j = 0; j < gra.size(); ++j)
			{
				PartOfSpeech curPos = PartOfSpeech(gra[j]);
				if (j == 0)
				{
					_grammarLocalTable[curPos]->AddForward(PartOfSpeech(gra[j + 1]));
				}
				else if (j == gra.size() - 1)
				{
					_grammarLocalTable[curPos]->AddBackward(PartOfSpeech(gra[j - 1]));
				}
				else
				{
					_grammarLocalTable[curPos]->AddForward(PartOfSpeech(gra[j + 1]));
					_grammarLocalTable[curPos]->AddBackward(PartOfSpeech(gra[j - 1]));
				}
			}
		}
	}


	void GrammarLocalModel::ReadGrammarLocal()
	{
		ifstream in(GetHopeLoveMindPath() + GrammarLocal_InitialFilename);
		if (!in)
		{
			LOG("Cannot find file: " + GrammarLocal_InitialFilename);
			return;
		}

		//The total POS for local grammar analysis is 15, including punctuations.
		int NUM_POS_FOR_LOCAL = NUM_PARTOFSPEECH + 3;

		//Initialize each POS with GrammarLocal.
		map<PartOfSpeech, GrammarLocal> grammarLocalTable;
		for (int i = 0; i < NUM_POS_FOR_LOCAL; ++i)
		{
			shared_ptr<GrammarLocal> grammarLocal(new GrammarLocal(PartOfSpeech(i)));
			grammarLocal->Read(in);
			_grammarLocalTable[PartOfSpeech(i)] = grammarLocal;
		}
	}

	double GrammarLocalModel::GetP_Forward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& forward) const
	{
		return _grammarLocalTable.at(me)->GetRatio_Forward(forward);
	}

	double GrammarLocalModel::GetP_Backward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& backward) const
	{
		return _grammarLocalTable.at(me)->GetRatio_Backward(backward);
	}

	double GrammarLocalModel::ComputeP_GrammarLocal(const PartOfSpeech& curPos, const PartOfSpeech& forwardPos, const PartOfSpeech& backwardPos) const
	{
		double p_cur_for = GetP_Forward(curPos, forwardPos);
		double p_cur_back = GetP_Backward(curPos, backwardPos);
		double p_for_cur = GetP_Backward(forwardPos, curPos);
		double p_back_cur = GetP_Forward(backwardPos, curPos);


		return (p_cur_for*p_for_cur + p_cur_back*p_back_cur) / 2;
	}

	map<double, PartOfSpeech> GrammarLocalModel::ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const
	{
		map<double, PartOfSpeech> res;

		for (int i = 0; i < NUM_PARTOFSPEECH; ++i)
		{
			double possi = ComputeP_GrammarLocal((PartOfSpeech)i, forwardPos, backwardPos);
			res[possi] = (PartOfSpeech)i;
		}

		return res;
	}
}

