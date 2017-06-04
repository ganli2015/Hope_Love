#pragma once
#include "InOut.h"

namespace CommonTool
{
	class DBoperator;
	class DBCmd;
}


namespace Mind
{
	class MindElementCreator;

	//////////////////////////////////////////////////////////////////////////
	//The base class of Database classes for Mind.
	//////////////////////////////////////////////////////////////////////////
	class _MINDINOUT MindDatabase
	{
		friend class Test_Database;
		friend class Test_Database_Normal;
	protected:
		CommonTool::DBoperator *_db;
		MindElementCreator *_elemCreator;
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


