#include "Stdafx.h"
#include "iCerebrum.h"

namespace Mind
{

	iCerebrum* iCerebrum::_instance = NULL;


	iCerebrum* iCerebrum::Instance()
	{
		return _instance;
	}

	void iCerebrum::SetInstance( iCerebrum* val )
	{
		_instance=val;
	}

	void iCerebrum::KillInstance()
	{
		if(_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}
	}

	bool iCerebrum::Empty()
	{
		if(_instance==NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

