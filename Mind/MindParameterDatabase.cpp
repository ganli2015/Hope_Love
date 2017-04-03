#include "stdafx.h"
#include "MindParameterDatabase.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"
#include "../CommonTools/LogWriter.h"

using namespace CommonTool;

namespace Mind
{
	MindParameterDatabase::MindParameterDatabase() :TableName("MindParam"), paramIDTag("name"), paramValueTag("value")
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

			UpdateWeight(id, weights[i]);
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

	void MindParameterDatabase::PrepareFeatureWeightID()
	{
		int weightCount = 5;
		for (unsigned i = 0; i < weightCount; ++i)
		{
			//Get id of the weight.
			char buffer[10];
			sprintf_s(buffer, "fw%d", i);
			string id(buffer);

			_featureWeightID.push_back(id);
		}
	}

	void MindParameterDatabase::UpdateWeight(const string id, const double weight)
	{
		char state[100];
		sprintf_s(state, "UPDATE %s SET %s = '%s' WHERE %s = '%s'", TableName.c_str(),
			paramValueTag.c_str(), ToString(weight).c_str(), paramIDTag.c_str(), id.c_str());

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
			auto valueStr = rows[0].GetText("value");
			return StringToNum<double>(valueStr);
		}
		else
		{
			throw runtime_error("Invalid id: " + id);
		}
	}

}

