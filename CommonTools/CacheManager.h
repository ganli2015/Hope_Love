#pragma once
#include "InOut.h"

#include "IJob.h"
#include "ThreadWorker.h"
#include "LogWriter.h"
#include "NonCopyable.h"
#include "Common.h"

namespace CommonTool
{
	//////////////////////////////////////////////////////////////////////////
	//Store cache for any objects.
	//Cache objects can be released or retrieved from <me>.
	//////////////////////////////////////////////////////////////////////////
	template<class ObjType>
	class CacheManager : public NonCopyable
	{
		class CacheMonitor;

		shared_ptr<CacheMonitor> _cacheMonitor;
		ThreadWorker* _bwWorker;
		map<string, shared_ptr<ObjType>> _objs;

	public:
		CacheManager() : _bwWorker(NULL) ,_cacheMonitor(NULL)
		{
		};
		virtual ~CacheManager()
		{
			//Delete CacheMonitor
			StopMonitor();

			//Delete thread.
			//Delete thread before CacheMonitor as thread holds pointer of CacheMonitor.
			if (_bwWorker != NULL)
			{
				delete _bwWorker;
				_bwWorker = NULL;
			}

		};

		//////////////////////////////////////////////////////////////////////////
		//Add object to cashe.Set a key for getting this object.
		//////////////////////////////////////////////////////////////////////////
		void Add(const string key, const shared_ptr<ObjType>& obj)
		{
			_objs[key] = obj;
		}

		shared_ptr<ObjType> Get(const string key) const
		{
			auto obj = _objs.find(key);
			if (obj != _objs.end())
			{
				return (*obj).second;
			}
			else
			{
				return NULL;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//Release all objects in <me>.
		//////////////////////////////////////////////////////////////////////////
		void ReleaseAll()
		{
			_objs.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		//Set that how long to raise release of objects in <me>.
		//If not set, the objects will never be released.
		//////////////////////////////////////////////////////////////////////////
		void SetReleaseInterval(const int second)
		{
			InitMonitor();
			_cacheMonitor->SetMonitorInterval(second);
		}

		void RunMonitor()
		{
			if (_cacheMonitor != NULL && _cacheMonitor->IsRunning())
			{
				return;
			}

			InitMonitor();

			//Create a shared pointer passed to ThreadWorker.
			shared_ptr<CacheMonitor> sharedMonitor(_cacheMonitor);
			_bwWorker = new ThreadWorker(sharedMonitor);
			_bwWorker->Start();
		}

	private:

		//////////////////////////////////////////////////////////////////////////
		//Raise when the cache is monitored after an interval.
		//The interval is set by calling <SetReleaseInterval>.
		//////////////////////////////////////////////////////////////////////////
		virtual void OnMonitor() = 0;

		void InitMonitor()
		{
			if (_cacheMonitor == NULL)
			{
				_cacheMonitor = make_shared<CacheMonitor>();
				_cacheMonitor->SetCacheManager(this);
			}
		}

		void StopMonitor()
		{
			if (_cacheMonitor != NULL)
			{
				if (_cacheMonitor->IsRunning())
				{
					_cacheMonitor->Cancel();
					while (_cacheMonitor->IsRunning())
					{
						SleepForSeconds(2);
					}
				}
			}
		}

	private:

		//////////////////////////////////////////////////////////////////////////
		//Monitor status of cache and do something.
		//////////////////////////////////////////////////////////////////////////
		class CacheMonitor: public IJob
		{
			int _interval;
			bool _cancelled;
			bool _running;

			CacheManager<ObjType>* _cacheManager;
		public:
			CacheMonitor() :_cancelled(false),
				_interval(10),
				_running(false)
			{
			};
			~CacheMonitor() {};

			void SetMonitorInterval(const int second) { _interval = second; }
			void SetCacheManager(CacheManager<ObjType>* cacheManager) { _cacheManager = cacheManager; }

			virtual void Run()
			{
				const int loopInterval = 1;
				int periodDuration = 0;

				_running = true;
				while (true)
				{
					try
					{
						if (_cancelled) break;

						SleepForSeconds(loopInterval);

						if (_cancelled) break;

						periodDuration += loopInterval;
						if (periodDuration > _interval)
						{
							_cacheManager->OnMonitor();
						}
					}
					catch (const std::exception&)
					{
						break;
					}
					
				}

				_running = false;
			}

			void Cancel()
			{
				_cancelled = true;
			}

			bool IsRunning() { return _running; }
		};
	};

}


