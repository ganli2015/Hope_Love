#include "stdafx.h"
#include "MindParameterDatabase.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

using namespace CommonTool;

namespace Mind
{
	MindParameterDatabase::MindParameterDatabase(const string dbPath, const int grammarFeatureTypeCount) :MindDatabase(dbPath),
		_grammarFeatureTypeCount(grammarFeatureTypeCount),
		TableName("MindParam"),
		ParamIDTag("name"),
		ParamValueTag("value"),
		FeaturegwID("featuregw"),
		LocalgwID("localgw"),
		PatterngwID("patterngw")
	{
		PrepareFeatureWeightID();
	}


	MindParameterDatabase::~MindParameterDatabase()
	{
	}

	void MindParameterDatabase::UpdateGrammarFeatureWeights(const vector<double> &weights)
	{
		CheckConnect();
		
		for (unsigned i=0;i<weights.size();++i)
		{
			//Get id of the weight.
			char buffer[10];
			sprintf_s(buffer, "fw%d", i);
			string id(buffer);

			UpdateValue(id, weights[i]);
		}
	}

	vector<double> MindParameterDatabase::GetGrammarFeatureWeights() 
	{
		CheckConnect();

		vector<double> res;
		for (auto id :_featureWeightID)
		{
			double value = GetDoubleParam(id);
			res.push_back(value);
		}
		return res;
	}

	void MindParameterDatabase::WriteGrammarFeatureModelWeight(const double val)
	{
		CheckConnect();

		UpdateValue(FeaturegwID, val);
	}

	double MindParameterDatabase::GetGrammarFeatureModelWeight()
	{
		CheckConnect();

		return GetDoubleParam(FeaturegwID);
	}

	void MindParameterDatabase::WriteGrammarLocalModelWeight(const double val)
	{
		CheckConnect();

		UpdateValue(LocalgwID, val);
	}

	double MindParameterDatabase::GetGrammarLocalModelWeight()
	{
		CheckConnect();

		return GetDoubleParam(LocalgwID);
	}

	void MindParameterDatabase::WriteGrammarPatternModelWeight(const double val)
	{
		CheckConnect();

		UpdateValue(PatterngwID, val);
	}

	double MindParameterDatabase::GetGrammarPatternModelWeight()
	{
		CheckConnect();

		return GetDoubleParam(PatterngwID);
	}

	void MindParameterDatabase::PrepareFeatureWeightID()
	{
		unsigned weightCount = _grammarFeatureTypeCount;
		for (unsigned i = 0; i < weightCount; ++i)
		{
			//Get id of the weight.
			char buffer[20];
			sprintf_s(buffer, "fw%d", i);
			string id(buffer);

			_featureWeightID.push_back(id);
		}
	}

	void MindParameterDatabase::UpdateValue(const string id, const double value)
	{
		char state[100];
		sprintf_s(state, "UPDATE %s SET %s = '%s' WHERE %s = '%s'", TableName.c_str(),
			ParamValueTag.c_str(), ToString(value).c_str(), ParamIDTag.c_str(), id.c_str());

		DBCmd cmd(state, *_db);
		cmd.Execute();
	}

	double MindParameterDatabase::GetDoubleParam(const string id) const
	{
		char state[100];
		sprintf_s(state, "Select * from %s where name='%s'", TableName.c_str(), id.c_str());

		DBQry qry(state, *_db);
		auto rows = qry.GetRows();
		if (rows.size() == 1)
		{
			//get unique value.
			auto valueStr = rows[0].GetText(ParamValueTag);
			return StringToNum<double>(valueStr);
		}
		else
		{
			throw runtime_error("Invalid id: " + id);
		}
	}

}

