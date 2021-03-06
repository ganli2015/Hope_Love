#pragma once
#include "InOut.h"

namespace Math
{
	enum state_sequence
	{
		none,
		up,
		left,
		diagonal
	};

	//Introduction to Algorithm，P210
	_MATHMATICINOUT void FindLongestCommonSubsequence
		(const std::vector<int>& seq1, 
		const std::vector<int>& seq2,
		std::vector<int>& common_seq);

	_MATHMATICINOUT void FindAllCommonSubsequence
		(const std::vector<int>& seq1, 
		const std::vector<int>& seq2,
		std::vector<std::vector<int>>& common_seqs);

	//Introduction to Algorithm，P570
	_MATHMATICINOUT bool IsMatch(string P,string T);

	_MATHMATICINOUT int MatchCount(string P,string T);

	vector<int> ComputePrefixFunction(string P);

	void ConstructSequenceTable(const std::vector<int>& seq1, 
		const std::vector<int>& seq2,state_sequence **b);

	void PrintLCS
		( state_sequence **b,const std::vector<int>& seq,
		const int i_start, const int j_start,
		const int i_end,const int j_end, 
		std::vector<int>& out);

	///Check whether <sub> is the (discontinuous) sub sequence of <full>
	template<class T>
	bool IsSubsequence(const std::vector<T>& sub, const std::vector<T>& full)
	{
		if(sub.empty()) return true;
		if(full.empty()) return false;

		size_t subIndex(0);
		T curElem=sub[subIndex];
		for (size_t i=0;i<full.size();++i)
		{
			if(curElem==full[i])
			{
				if(++subIndex<sub.size())
				{
					curElem=sub[subIndex];
				}
				else
				{
					return true;
				}
			}
		}

		return false;
	}

	template<class T>
	class GetAllCombinations
	{
		class RecusiveTerminate
		{
		public:
			RecusiveTerminate(){}
			~RecusiveTerminate(){}
		};

	public:
		///<val> is an ordered sequence and every element <val[i]> is the several possibilities of the ith element.
		///Then get all the possible combinations over every possibility of every element in  <val[i]>.
		static vector<vector<T>> Get(const vector<vector<T>>& val)
		{
			vector<vector<T>> res;
			try
			{
				Recursive(0,val,res);
			}
			catch(RecusiveTerminate& e)
			{
				e.~RecusiveTerminate();
				res.clear();
			}

			return res;
		}

	private:
		static void Recursive(const size_t index, const vector<vector<T>>& in, vector<vector<T>>& out)
		{
			if(index>=in.size()) return;

			vector<T> curElem=in[index];
			if(curElem.empty()) 
			{
				throw RecusiveTerminate();
			}

			vector<vector<T>> newout;
			for (size_t i=0;i<curElem.size();++i)
			{
				if(out.empty())//对于第一次递归，只需要添加in的第一列到out中
				{
					vector<T> newSeq;
					newSeq.push_back(curElem[i]);
					newout.push_back(newSeq);
				}
				else//在之前已经创建的组合的基础上添加当前元素，组合的个数会乘以curElem.size()倍（除了0）。
				{
					for (size_t j=0;j<out.size();++j)
					{
						vector<T> newSeq=out[j];
						newSeq.push_back(curElem[i]);
						newout.push_back(newSeq);
					}
				}
			}

			out=newout;
			//对下一列递归
			Recursive(index+1,in,out);
		}
	};

	template<class T>
	class GetAllSubSequence
	{
	public:
		///Find all (discontinuous)  sub sequences of seq.
		static vector<vector<T>> Get(const vector<T>& seq)
		{
			vector<vector<T>> res;
			for (size_t i=1;i<=seq.size();++i)
			{
				vector<vector<T>> subs;
				FindSubSequence(seq,i,0,subs);
				res.insert(res.end(),subs.begin(),subs.end());
			}

			return res;
		}
	private:
		static void FindSubSequence(const vector<T>& raw,const size_t length,const size_t startIndex,vector<vector<T>>& out)
		{
			if(raw.size()-startIndex<length) return;
			if(length<=0) return;

			out.clear();
			for (size_t i=startIndex;i<raw.size();++i)
			{			
				//从raw的第i+1到结尾处，寻找长度等于length-1的子序列。
				vector<vector<T>> subsubs;
				FindSubSequence(raw,length-1,i+1,subsubs);

				if(length==1)//如果长度等于1，subsubs必然是空的，所以特殊处理。
				{
					vector<T> sub;
					sub.push_back(raw[i]);
					out.push_back(sub);
					continue;
				}

				//把当前的元素和子结构里的length-1长度的子序列连接起来，形成长度为length的子序列。
				for (size_t j=0;j<subsubs.size();++j)
				{
					vector<T> sub;
					sub.reserve(length);
					sub.push_back(raw[i]);
					sub.insert(sub.end(),subsubs[j].begin(),subsubs[j].end());

					out.push_back(sub);
				}
			}
		}
	};
}