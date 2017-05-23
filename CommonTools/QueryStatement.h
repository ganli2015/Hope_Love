#pragma once
#include "InOut.h"

namespace CommonTool
{
	//////////////////////////////////////////////////////////////////////////
	//Statement of query for database.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT QueryStatement
	{
		string _tableName;

		//Equality constraints.Key is field name.
		vector<pair<string, string>> _eqConditionsString;
		vector<pair<string, int>> _eqConditionsInt;
		vector<pair<string, long>> _eqConditionsLong;
		//Like constraints.
		vector<pair<string, string>> _likeConditions;//key is field name and value is like format.

	public:
		QueryStatement(const string tableName);
		~QueryStatement();

		//////////////////////////////////////////////////////////////////////////
		//Generate sql sentence from <me>.
		//////////////////////////////////////////////////////////////////////////
		string GetString() const;

		//////////////////////////////////////////////////////////////////////////
		//Set equality condition.
		//The field value equals to <val>.
		//<val> should be encoded as Ascii !!!
		//////////////////////////////////////////////////////////////////////////
		void EQ(const string fieldName, const string val);
		void EQ(const string fieldName, const int val);
		void EQ(const string fieldName, const long val);

		//////////////////////////////////////////////////////////////////////////
		//Only one Like contraint will be set.
		//////////////////////////////////////////////////////////////////////////
		void Like(const string fieldName,const string format);
	};
}

