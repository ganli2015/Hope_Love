#pragma once
#include "InOut.h"
#include "Reaction.h"

namespace DataCollection
{
	class Sentence;
}

class _REACTIONLISTINOUT SpeakReaction:public Reaction
{
	vector<shared_ptr<DataCollection::Sentence>> _sentence_input;
	vector<shared_ptr<DataCollection::Sentence>> _sentence_output;

public:
	SpeakReaction(void);
	~SpeakReaction(void);

	SpeakReaction(const vector<shared_ptr<DataCollection::Sentence>> sentence);

	virtual void React();

	shared_ptr<DataCollection::Sentence> GetReactSentence() const;
};

