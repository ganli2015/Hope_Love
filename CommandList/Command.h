#pragma once
#include "InOut.h"
#include <functional>

namespace DataWrapperCPP
{
	class DataWrapper_Sentence;
}

class _COMMANDINOUT Command : public Obj<Command>
{
public:
	Command(void);
	virtual ~Command(void);

	virtual void Update()=0;

private:
	void UpdateConfigureManager() const;

};

