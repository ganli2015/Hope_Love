#pragma once
#include "InOut.h"

namespace DataCollection
{
	class Character;

	///Punctures is a collection of punctures.
	///It only supports for Chinese punctures.
	class _DATACOLLECTIONINOUT Punctuations : public Obj<Punctuations>
	{
		std::vector<shared_ptr<Character>> _punctures;
		std::vector<shared_ptr<Character>> _puncEndofSentence;
		std::vector<shared_ptr<Character>> _puncRightside;
		std::vector<shared_ptr<Character>> _puncLeftside;

		void Initialize();
	public:
		Punctuations(void);
		~Punctuations(void);

		///A puncture of end of the sentence is some puncture that separate two sentences or two sub sentences.
		///For example, '��' '��'.
		bool IsPuncEndofSentence(shared_ptr<Character> val) const;
		///A puncture of right side is some puncture that appears in the middle of the sentences.
		///For example, '��'
		bool IsPuncRightside(shared_ptr<Character> val) const;
		bool IsPuncLeftside(shared_ptr<Character> val) const;
	};
}


