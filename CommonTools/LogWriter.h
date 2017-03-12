#pragma once
#include "InOut.h"
#include <fstream>
#include <list>
#include <time.h>

namespace log4cpp
{
	class Category;
}

namespace CommonTool
{
	enum LogLevel
	{
		Debug,
		Information,
		Warn,
		Error
	};

	///Write log file.
	class _COMMONTOOLSINOUT LogWriter
	{
	private:
		///The default time when pushing enter key.
		static double _startTime;

		static LogWriter* _instance;

		log4cpp::Category* _root;

	private:
		///The current log file.
		static ofstream _currentLog;

		template<class T>
		class OutputObjectVal
		{
			LogLevel _level;

		public:
			OutputObjectVal(LogLevel val):_level(val){}

			void operator()(const T& obj)
			{
				LogWriter::GetInstance()->Log(obj.GetString(), _level);
			}
		};

		template<class T>
		class OutputObjectPointer
		{
			LogLevel _level;
		public:
			OutputObjectPointer(LogLevel val) :_level(val) {}
			void operator()(const T& obj)
			{
				LogWriter::GetInstance()->Log(obj->GetString(),_level);
			}
		};

	public:
		static LogWriter* GetInstance();
		~LogWriter(void);

		///General
		template<class T> 
		static void Output(const T object,LogLevel level)
		{
			OutputObjectVal<T> output(level);
			output(object);
		}

		///For string
		template<> 
		static void Output(const string str, LogLevel level)
		{
			LogWriter::GetInstance()->Log(str,level);
		}

		///For const char
		template<> 
		static void Output(const char* str, LogLevel level)
		{
			string chStr(str);
			Output(chStr,level);
		}

		///For shared_ptr
		template<class T> 
		static void Output(const shared_ptr<T> object, LogLevel level)
		{
			OutputObjectPointer<shared_ptr<T>> output(level);
			output(object);
		}

		///For vector
		template<class T> 
		static void Output(const vector<T> objects, LogLevel level)
		{
			OutputObjectVal<T> output(level);

			for_each(object.begin(),object.end(), output);
		}

		///For list shared_ptr
		template<class T> 
		static void Output(const list<shared_ptr<T>> objects, LogLevel level)
		{
			OutputObjectPointer<shared_ptr<T>> output(level);
			for_each(objects.begin(), objects.end(), output);
		}

		///For vector shared_ptr
		template<class T> 
		static void Output(const vector<shared_ptr<T>> objects, LogLevel level)
		{
			OutputObjectPointer<shared_ptr<T>> output(level);
			for_each(objects.begin(), objects.end(), output);
		}

		///Output with format.
		template<class T>
		static void OutFormat(const char *format,const LogLevel level, T var1)
		{
			int bufferSize = 255;
			char* ch = new char[bufferSize];
			sprintf_s(ch, bufferSize, format, var1);
			Output(string(ch), level);

			delete[] ch;
		}


		static void OutputException(const exception& ex);

		static void ResetStartTime();

	private:

		//////////////////////////////////////////////////////////////////////////
		//Create default LogWriter.
		//////////////////////////////////////////////////////////////////////////
		LogWriter();

		void Initialize();

		static void Log(string str, LogLevel level);

		void Debug(const string str);
		void Warn(const string str);
		void Error(const string str);
		void Info(const string str);
		void DebugFormat(const char* str,...);
		void WarnFormat(const char* str, ...);
		void ErrorFormat(const char* str, ...);
		void InfoFormat(const char* str, ...);

		static double GetDuration() ;
	};

	///Record time durations of each code section.
	class _COMMONTOOLSINOUT EfficiencyRecorder
	{
		///The section key and its total duration.
		static map<string, double> _section_duration;
		const static string _outFilename;

		friend class CodeSection;
	public:
		EfficiencyRecorder();
		~EfficiencyRecorder();

		static void Output();
		static void Clear() { _section_duration.clear(); }
	};


	class _COMMONTOOLSINOUT CodeSection
	{
		string _secName;
		double _startTime;
		bool _released;
	public:
		CodeSection(const string secName);
		~CodeSection();

		void Release();
	};

	
}



#define _USE_LOG

#ifdef _USE_LOG

/************************************************************************/
/* Log information                                                                     */

///Output object.<Object> must have a method of GetString().
#define LOG(object) CommonTool::LogWriter::Output(object,CommonTool::Information) 

#define LOG_FORMAT(format,var) CommonTool::LogWriter::OutFormat(format,CommonTool::Information,var) 

///Write information of object as well as its description.
#define LOG_DESC(desc,object) LOG(desc);LOG(object)

//Log under some condition.
#define LOG_IF(condition,object) if(condition) LOG(object)

#define LOG_IF_FORMAT(condition,format,var) if(condition) {LOG_FORMAT(format,var);}

/************************************************************************/
//Log Debug

#define DEBUGLOG(object) CommonTool::LogWriter::Output(object,CommonTool::Debug) 

#define DEBUG_DESC(desc,object) DEBUGLOG(desc);DEBUGLOG(object)

#define DEBUG_IF(condition,desc,object) if(condition) {DEBUG_DESC(desc,object);}

#define DEBUG_FORMAT(format,var) CommonTool::LogWriter::OutFormat(format,CommonTool::Debug,var) 

/************************************************************************/

#define LOG_EXCEPTION(ex) CommonTool::LogWriter::OutputException(ex);

//#define CREATELOG(filename) CommonTool::LogWriter NEWLOG(filename)

///Create a section and record the time consuming of it.
#define SECTION_TIME(tag) CommonTool::CodeSection tag##CodeSection(#tag)
#define END_SECTION(tag) tag##CodeSection.Release()

#else 
#define LOG_DESC(object)
#endif // _USE_LOG

