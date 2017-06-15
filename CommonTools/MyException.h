#pragma once
#include "InOut.h"


namespace CommonTool
{
	///IterationDiverge represents divergence in an iteration process(for, while...)
	class _COMMONTOOLSINOUT IterationDiverge : public runtime_error
	{
	public:
		IterationDiverge();
		~IterationDiverge();

	private:

	};

	//////////////////////////////////////////////////////////////////////////
	//Exception for database functions.
	//////////////////////////////////////////////////////////////////////////
	class _COMMONTOOLSINOUT DatabaseException : public exception
	{
		string _tableName;
		string _msg;
	public:
		DatabaseException();
		DatabaseException(string tableName,string msg="");
		DatabaseException(const DatabaseException&);
		~DatabaseException() {};

		virtual char const* what() const;
	};
}

