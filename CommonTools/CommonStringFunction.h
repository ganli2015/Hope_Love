#pragma once
#include "InOut.h"
#include <sstream>
#include <fstream>

namespace CommonTool
{
	//����<splitTag>�ָ��ַ���<str>
	_COMMONTOOLSINOUT vector<string> SplitString(const string& str, const char splitTag);

	_COMMONTOOLSINOUT void TrimBeginEndBlank(string& str);

	//Convert to string.
	template<class T>
	string ToString(const T t)
	{
		stringstream ss;
		ss.precision(13);
		ss<<t;
		return ss.str();
	}

	template <class Type>
	Type StringToNum(const string& str)
	{
		istringstream iss(str);
		Type num;
		iss >> num;
		return num;
	}

	_COMMONTOOLSINOUT int StrToInt(const string str);

	//**************string******************//  
	// ASCII��Unicode��ת  
	_COMMONTOOLSINOUT wstring AsciiToUnicode(const string& str);
	_COMMONTOOLSINOUT string  UnicodeToAscii(const wstring& wstr);
	// UTF8��Unicode��ת  
	_COMMONTOOLSINOUT wstring Utf8ToUnicode(const string& str);
	_COMMONTOOLSINOUT string  UnicodeToUtf8(const wstring& wstr);
	// ASCII��UTF8��ת  
	_COMMONTOOLSINOUT string  AsciiToUtf8(const string& str);
	_COMMONTOOLSINOUT string  Utf8ToAscii(const string& str);
	//**************CString******************//  
	// ASCII��Unicode��ת  

	//////////////////////////////////////////////////////////////////////////
	//Get one line of a Unicode file and parse the line to Ascii.
	//Note!The index should start from 2 and it is an output from this function.
	//////////////////////////////////////////////////////////////////////////
	_COMMONTOOLSINOUT std::string Getline_UnicodeFile(ifstream& fin, size_t& index);

	//////////////////////////////////////////////////////////////////////////
	//Get hash value of a string.
	//////////////////////////////////////////////////////////////////////////
	_COMMONTOOLSINOUT size_t GetStrHash(const string str);

	//////////////////////////////////////////////////////////////////////////
	//Return a string with format input.
	//////////////////////////////////////////////////////////////////////////
	_COMMONTOOLSINOUT char* StringFormat(const char * format, ...);

	//////////////////////////////////////////////////////////////////////////
	//Generate unique ID from a string.
	//////////////////////////////////////////////////////////////////////////
	_COMMONTOOLSINOUT string GenerateID(const string str);
}

