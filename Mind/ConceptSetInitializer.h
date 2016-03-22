#pragma once
#include "InOut.h"

#include <iostream>

namespace DataCollection
{
	class Word;
}

namespace Mind
{
	class ConceptSet;
	struct Identity;
	struct Connection_Info;

	class ConceptSetInitializer
	{
		struct Word_ID
		{
			shared_ptr<DataCollection::Word> word;
			//区别同一个单词的不同词性.目前同一个单词的同一个词性只对应于一个id！
			//也就是说这个id暂时起不到作用！
			int id;
		};

	public:
		ConceptSetInitializer(void);
		~ConceptSetInitializer(void);

		static void InitializeNonBaseConcept(ConceptSet* conceptSet,const string filename_concept);

		static void InitializeBaseConcept(ConceptSet* conceptSet,const string filename);		

		static void InitializeConceptConnection(ConceptSet* conceptSet,string filename);

		static void CheckInitialConceptData();

	private:
		//初始化ConceptSet时使用
		static vector<Word_ID> InputWordFromFile( string filename );

		//检查wholeConcepts里是否有重复的单词（相同字符串同时ID也相同的单词）.
		static void CheckNonBaseConceptString(const vector<Word_ID>& wholeConcepts,ofstream& out);
		//检查connectionInfos里所有涉及的Concept是否存在于wholeConcepts中.
		static void CheckConceptConnection(const vector<Connection_Info>& connectionInfos,
			const vector<Word_ID>& wholeConcepts, 
			const vector<Word_ID>& baseConcepts,
			ofstream& out);
		//检查base里是否有重复的单词，检查在nonBaseConcepts里是否有base里的单词.
		static void CheckBaseConcept(const vector<Word_ID>& base,const vector<Word_ID>& nonBaseConcepts,ofstream& out);

		static bool ExistDuplicatedIDAndPOS(const Word_ID word_id,const int index,const vector<Word_ID>& list,Word_ID& duplicated);
		static void OutputWordID(const Word_ID w_i,std::ofstream& out);
		static void OutputIdentity( const Identity identity,std::ofstream& out );
		static bool IdentityExist(const Identity identity,const vector<Word_ID>& list);
		static bool WordIDExist(const Word_ID word_id,const vector<Word_ID>& list);

	};
}
