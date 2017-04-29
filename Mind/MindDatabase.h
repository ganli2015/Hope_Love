#pragma once
#include "InOut.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}


namespace Mind
{
	//////////////////////////////////////////////////////////////////////////
	//The base class of Database classes for Mind.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINOUT MindDatabase
	{
		friend class Test_Database;
	protected:
		CommonTool::DBoperator *_db;
	public:
		MindDatabase();
		~MindDatabase();

		void Connect();
		void Disconnect();

	protected:
		//////////////////////////////////////////////////////////////////////////
		//Check if database is connected.
		//If not, then connect.
		//////////////////////////////////////////////////////////////////////////
		void CheckConnect();
	};
}


