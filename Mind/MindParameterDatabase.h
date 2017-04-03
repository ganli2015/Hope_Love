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
		vector<string> _featureWeightID;

	public:
		MindParameterDatabase();
		~MindParameterDatabase();

		//////////////////////////////////////////////////////////////////////////
		//Write grammar feature weights to database.
		//////////////////////////////////////////////////////////////////////////
		void UpdateGrammarFeatureWeights(const vector<double> &weights);

		vector<double> GetGrammarFeatureWeights();

	private:

		void PrepareFeatureWeightID();

		void UpdateWeight(const string id, const double weight);
		//////////////////////////////////////////////////////////////////////////
		//Get a double value from database.
		//////////////////////////////////////////////////////////////////////////
		double GetDoubleParam(const string id) const;
	};
}


