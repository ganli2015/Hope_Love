#pragma once
#include "InOut.h"
#include <sstream>

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
		ss<<t;
		return ss.str();
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

}

