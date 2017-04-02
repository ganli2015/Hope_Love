#pragma once
#include "MindDatabase.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}

namespace Mind
{
	class MindParameterDatabase : public MindDatabase
	{
		const string TableName;
		const string paramIDTag;
		const string paramValueTag;

		CommonTool::DBoperator *_db;
	public:
		MindParameterDatabase();
		~MindParameterDatabase();

		void UpdateGrammarFeatureWeights(const vector<double> &weights);

	private:

		void UpdateWeight(const string id, const double weight);
	};
}


