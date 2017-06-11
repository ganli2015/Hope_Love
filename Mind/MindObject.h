#pragma once
#include "InOut.h"

#include "../CommonTools/NonCopyable.h"

#include "../MindDatabase/DBContainer.h"

namespace Mind
{
	class DBContainer;
	class MindElementCreator;
	class ConceptReadWriter;

	//////////////////////////////////////////////////////////////////////////
	//The base for Mind classes used for accessing the same object.
	//////////////////////////////////////////////////////////////////////////
	class MindObject : public CommonTool::NonCopyable
	{
	protected:
		DBContainer* _dbContainer;
		MindElementCreator* _elemCreator;
	public:
		MindObject();
		~MindObject();
	};

	class CachedDBContainer : public DBContainer
	{
		ConceptReadWriter *_conceptReadWriter;
	public:
		CachedDBContainer(const string dbPath);
		virtual ~CachedDBContainer();

		virtual unique_ptr<ConceptDatabase> GetConceptDatabase() const;

	};
}


