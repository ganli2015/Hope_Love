#pragma once
#include "MindDatabase.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}

namespace Mind
{
	class _MINDINOUT MindParameterDatabase : public MindDatabase
	{
		const string TableName;
		const string ParamIDTag;
		const string ParamValueTag;
		vector<string> _featureWeightID;
		const string FeaturegwID;
		const string LocalgwID;
		const string PatterngwID;

		friend class Test_Database;
	public:
		MindParameterDatabase();
		~MindParameterDatabase();

		//////////////////////////////////////////////////////////////////////////
		//Write grammar feature weights to database.
		//////////////////////////////////////////////////////////////////////////
		void UpdateGrammarFeatureWeights(const vector<double> &weights);

		vector<double> GetGrammarFeatureWeights();

		//////////////////////////////////////////////////////////////////////////
		//Functions below are used for writing and reading weights for different grammar models.
		void WriteGrammarFeatureModelWeight(const double val);
		double GetGrammarFeatureModelWeight();

		void WriteGrammarLocalModelWeight(const double val);
		double GetGrammarLocalModelWeight();

		void WriteGrammarPatternModelWeight(const double val);
		double GetGrammarPatternModelWeight();

		//////////////////////////////////////////////////////////////////////////

	private:

		void PrepareFeatureWeightID();

		void UpdateValue(const string id, const double value);
		//////////////////////////////////////////////////////////////////////////
		//Get a double value from database.
		//////////////////////////////////////////////////////////////////////////
		double GetDoubleParam(const string id) const;
	};
}


