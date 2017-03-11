#include "StdAfx.h"
#include "LogWriter.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include <log4cpp/PatternLayout.hh>

namespace CommonTool
{
	double LogWriter::_startTime=(double)clock()/CLOCKS_PER_SEC;
	LogWriter* LogWriter::_instance = NULL;

	LogWriter::LogWriter()
	{
		Initialize();
	}

	LogWriter* LogWriter::GetInstance()
	{
		if (_instance == NULL)
		{
			_instance = new LogWriter();
		}

		return _instance;
	}

	LogWriter::~LogWriter(void)
	{
	}

	void LogWriter::ResetStartTime()
	{
		_startTime=clock()/CLOCKS_PER_SEC;
	}

	void LogWriter::Initialize()
	{
		//Initialize me with log4cpp.Referenced from http://log4cpp.sourceforge.net.
		log4cpp::Appender *appender1 = new log4cpp::OstreamAppender("console", &std::cout);
		appender1->setLayout(new log4cpp::BasicLayout());

		log4cpp::Appender *appender2 = new log4cpp::FileAppender("default", "Log\\loginfo.log");
		auto fileLayout = new log4cpp::PatternLayout();
		fileLayout->setConversionPattern("%d [%p] %m%n");
		appender2->setLayout(fileLayout);

		_root = &log4cpp::Category::getRoot();
		_root->setPriority(log4cpp::Priority::DEBUG);
		_root->addAppender(appender1);
		_root->addAppender(appender2);
	}

	void LogWriter::Debug(const string str)
	{
		_root->debug(str);
	}

	void LogWriter::Warn(const string str)
	{
		_root->warn(str);
	}

	void LogWriter::Error(const string str)
	{
		_root->error(str);
	}

	void LogWriter::Info(const string str)
	{
		_root->info(str);
	}

	void LogWriter::DebugFormat(const char* str, ...)
	{
		
	}
	void LogWriter::WarnFormat(const char* str, ...)
	{

	}

	void LogWriter::ErrorFormat(const char* str, ...)
	{

	}

	void LogWriter::InfoFormat(const char* str, ...)
	{

	}

	double LogWriter::GetDuration()
	{
		double duration=(double)clock()/CLOCKS_PER_SEC-_startTime;

		return duration;
	}

	std::map<std::string, double> EfficiencyRecorder::_section_duration;
	const string EfficiencyRecorder::_outFilename("Log//Performance Detect.txt");

	EfficiencyRecorder::EfficiencyRecorder()
	{

	}

	EfficiencyRecorder::~EfficiencyRecorder()
	{

	}

	void EfficiencyRecorder::Output()
	{
		ofstream out(_outFilename);
		for (map<std::string, double>::const_iterator it=_section_duration.begin();it!=_section_duration.end();++it)
		{
			out << it->first << "    " << it->second << endl;
		}

		out.close();
	}

	CodeSection::CodeSection(const string secName) :_secName(secName),_released(false)
	{
		_startTime = (double)clock() / CLOCKS_PER_SEC;
	}

	CodeSection::~CodeSection()
	{
		if(!_released)
		{
			Release();
		}
	}

	void CodeSection::Release()
	{
		double duration = (double)clock() / CLOCKS_PER_SEC - _startTime;

		//Check whether  EfficiencyRecorder contain same section.
		//If so , then add time duration based on previous time.
		if (EfficiencyRecorder::_section_duration.count(_secName) != 0)
		{
			EfficiencyRecorder::_section_duration[_secName] += duration;
		}
		else
		{
			EfficiencyRecorder::_section_duration[_secName] = duration;
		}

		_released = true;
	}

}

