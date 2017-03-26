#pragma once
#include "InOut.h"
#include <sstream>

namespace CommonTool
{
	//按照<splitTag>分割字符串<str>
	_COMMONTOOLSINOUT vector<string> SplitString(const string& str, const char splitTag);

	_COMMONTOOLSINOUT void TrimBeginEndBlank(string& str);

	//Convert to string.
	template<class T>
	string ToString(const T t)
	{
		stringstream ss;
		ss<<t;
		return ss.str();
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

}

