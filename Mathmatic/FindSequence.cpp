#include "StdAfx.h"
#include "FindSequence.h"

using namespace std;

namespace Math
{
	_MATHMATICINOUT void FindLongestCommonSubsequence( const std::vector<int>& seq1, const std::vector<int>& seq2, std::vector<int>& common_seq )
	{
		size_t m=seq1.size();
		size_t n=seq2.size();
		state_sequence **b=new state_sequence *[m+1];
		ConstructSequenceTable(seq1,seq2,b);
		common_seq.clear();
		PrintLCS(b,seq1,0,0,m,n,common_seq);

		for (size_t i=0;i<m+1;++i)
		{
			delete[] b[i];
		}
		delete[] b;
	}

	void ConstructSequenceTable( const std::vector<int>& seq1, const std::vector<int>& seq2,state_sequence **b )
	{
		size_t m=seq1.size();
		size_t n=seq2.size();
		size_t **c;
		c=new size_t *[m+1];
		
		for (unsigned i=0;i<=m;++i)
		{
			c[i]=new size_t[n+1];
			b[i]=new state_sequence[n+1];
		}
		for(size_t row = 0;row <= m;row++) 
		{  
			c[row][0] = 0; 
			b[row][0] = none;
		}
		for(size_t col = 0;col <= n;col++) 
		{  
			c[0][col] = 0; 
			b[0][col] = none;
		}

		for (size_t i=1;i<=m;++i)
		{
			for (size_t j=1;j<=n;++j)
			{
				if(seq1[i-1]==seq2[j-1])
				{
					c[i][j]=c[i-1][j-1]+1;
					b[i][j]=diagonal;
				}
				else if(c[i-1][j]>=c[i][j-1])
				{
					c[i][j]=c[i-1][j];
					b[i][j]=up;
				}
				else
				{
					c[i][j]=c[i][j-1];
					b[i][j]=left;
				}
			}
		}

		for (size_t i=0;i<m+1;++i)
		{
			delete[] c[i];
		}
		delete[] c;
	}

	void PrintLCS( state_sequence **b,const std::vector<int>& seq, const int i_start, const int j_start, const int i_end,const int j_end, std::vector<int>& out )
	{
		if(i_end==i_start || j_end==j_start)
			return;

		if(b[i_end][j_end]==diagonal)
		{
			PrintLCS(b,seq,i_start,j_start,i_end-1,j_end-1,out);
			out.push_back(seq[i_end-1]);
		}
		else if(b[i_end][j_end]==up)
		{
			PrintLCS(b,seq,i_start,j_start,i_end-1,j_end,out);
		}
		else
		{
			PrintLCS(b,seq,i_start,j_start,i_end,j_end-1,out);
		}
	}

	_MATHMATICINOUT void FindAllCommonSubsequence( const std::vector<int>& seq1, const std::vector<int>& seq2, std::vector<std::vector<int>>& common_seqs )
	{
		size_t m=seq1.size();
		size_t n=seq2.size();
		state_sequence **b=new state_sequence *[m+1];
		ConstructSequenceTable(seq1,seq2,b);
		
		common_seqs.clear();
		for (size_t i=1;i<=m;++i)
		{
			for (size_t j=1;j<=n;++j)
			{
				if(b[i][j]==diagonal)
				{
					vector<int> commonSeq;
					PrintLCS(b,seq1,i-1,j-1,m,n,commonSeq);
					if(commonSeq.size()>1)
						common_seqs.push_back(commonSeq);
				}
			}
		}

		for (size_t i = 0; i < m + 1; ++i)
		{
			delete[] b[i];
		}
		delete[] b;
	}

	_MATHMATICINOUT bool IsMatch( string P,string T )
	{
		size_t n=T.length();
		size_t m=P.length();
		vector<int> pi=ComputePrefixFunction(P);

		int q=0;
		for (size_t i=0;i<n;++i)
		{
			while(q>0 && P[q]!=T[i])
			{
				q=pi[q-1];
			}
			if(P[q]==T[i])
			{
				q++;
			}

			if(q==m)
			{
				return true;
			}
		}

		return false;
	}

	_MATHMATICINOUT int MatchCount( string P,string T )
	{
		size_t n=T.length();
		size_t m=P.length();
		vector<int> pi=ComputePrefixFunction(P);

		int q=0;
		int count(0);
		for (size_t i=0;i<n;++i)
		{
			while(q>0 && P[q]!=T[i])
			{
				q=pi[q-1];
			}
			if(P[q]==T[i])
			{
				q++;
			}

			if(q==m)
			{
				q=pi[q-1];
				count++;
			}
		}

		return count;
	}

	vector<int> ComputePrefixFunction( string P )
	{
		vector<int> res;
		int m=P.length();
		res.reserve(m);

		res.push_back(0);
		int k=0;
		for(int q=1;q<m;++q)
		{
			while(k>0 && P[k]!=P[q])
			{
				k=res[k-1];
			}
			if(P[k]==P[q])
			{
				k++;
			}
			res.push_back(k);
		}

		return res;
	}

	

}


