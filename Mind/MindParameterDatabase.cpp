#include "stdafx.h"
#include "MindParameterDatabase.h"

#include "../CommonTools/DBoperator.h"
#include "../CommonTools/CommonStringFunction.h"

using namespace CommonTool;

namespace Mind
{
	MindParameterDatabase::MindParameterDatabase() :TableName("MindParam"), paramIDTag("name"), paramValueTag("value"), _db(NULL)
	{
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

	void MindParameterDatabase::UpdateWeight(const string id, const double weight)
	{
		char state[100];
		sprintf_s(state, "UPDATE %s SET %s = %s WHERE %s = %s;", TableName.c_str(),
			paramValueTag.c_str(), ToString(weight).c_str(), paramIDTag.c_str(), id.c_str());

		DBCmd cmd(state, *_db);
		cmd.Execute();
	}

}

