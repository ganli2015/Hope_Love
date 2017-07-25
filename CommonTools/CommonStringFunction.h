#pragma once
#include "InOut.h"
#include <sstream>
#include <fstream>

namespace CommonTool
{
	//按照<splitTag>分割字符串<str>
	_COMMONTOOLSINOUT vector<string> SplitString(const string& str, const char splitTag);

	//Remove empty string element in vector.
	_COMMONTOOLSINOUT void RemoveEmptyString(vector<string>& vec);

	//Check if <str> starts with <tag>.
	_COMMONTOOLSINOUT bool StartWith(const string str, const char tag);

	//Find the location of <tag>.
	_COMMONTOOLSINOUT string::const_iterator FindChar(const string& str, const char tag);
	//Find the location at the front of the begin of <tag>.
	_COMMONTOOLSINOUT string::const_iterator FindString(const string& str, const string tag);
	//Find the location at the front of the begin of <tag>.
	//Search from <startPos>.
	_COMMONTOOLSINOUT string::const_iterator FindString(const string& str,const string::const_iterator& startPos, const string tag);
	//Check if <str> contains <target>.
	_COMMONTOOLSINOUT bool HasString(const string& str, const string& target);

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
	// ASCII与Unicode互转  
	_COMMONTOOLSINOUT wstring AsciiToUnicode(const string& str);
	_COMMONTOOLSINOUT string  UnicodeToAscii(const wstring& wstr);
	// UTF8与Unicode互转  
	_COMMONTOOLSINOUT wstring Utf8ToUnicode(const string& str);
	_COMMONTOOLSINOUT string  UnicodeToUtf8(const wstring& wstr);
	// ASCII与UTF8互转  
	_COMMONTOOLSINOUT string  AsciiToUtf8(const string& str);
	_COMMONTOOLSINOUT string  Utf8ToAscii(const string& str);
	//**************CString******************//  
	// ASCII与Unicode互转  

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
	_COMMONTOOLSINOUT string GenerateHash(const string str);
}

