#pragma once
#include "InOut.h"

namespace CommonTool
{
	class ConstraintStatement;

	//////////////////////////////////////////////////////////////////////////
	//Statement of update for database.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT UpdateStatement
	{
		string _tableName;

		//Cache of values to update.Key is field name.
		vector<pair<string, string>> _stringForUpdate;
		vector<pair<string, int>> _intForUpdate;
		vector<pair<string, long>> _longForUpdate;

		ConstraintStatement* _constraint;

	public:
		UpdateStatement(const string tableName);
		~UpdateStatement();

		//////////////////////////////////////////////////////////////////////////
		//Generate sql sentence from <me>.
		//////////////////////////////////////////////////////////////////////////
		string GetString() const;

		void Update(const string fieldName, const string val);
		void Update(const string fieldName, const int val);
		void Update(const string fieldName, const long val);

		/*---------------------------------------Constraints-------------------------------*/

		void EQ(const string fieldName, const string val);
		void EQ(const string fieldName, const int val);
		void EQ(const string fieldName, const long val);

		void Like(const string fieldName, const string format);

		/*--------------------------------------------------------------------------------*/

	};

}
