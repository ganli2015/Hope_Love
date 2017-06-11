#include "stdafx.h"
#include "ThreadWorker.h"

#include "IJob.h"

#include <functional>

namespace CommonTool
{
	ThreadWorker::ThreadWorker(shared_ptr<IJob> job):_thread(NULL),_job(job)
	{
	}


	ThreadWorker::~ThreadWorker()
	{
		if (_thread != NULL)
		{
			try
			{
				_thread->join();
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
			delete _thread;
			_thread = NULL;
		}
	}

	void ThreadWorker::Start()
	{
		_thread = new std::thread(std::mem_fn(&IJob::Run), _job);
	}

	void ThreadWorker::Wait()
	{
		if(_thread!=NULL)
			_thread->join();
	}

}
