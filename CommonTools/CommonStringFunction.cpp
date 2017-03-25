#include "StdAfx.h"
#include "CommonStringFunction.h"

#include "GeneralFunctor.h"


namespace CommonTool
{
	vector<string> SplitString( const string& str, const char splitTag )
	{
		vector<string> res;
		int startIndex=0;
		size_t found=str.find_first_of(splitTag,startIndex);//���ҷָ������
		while(found!=string::npos)
		{
			res.push_back(string(str.begin()+startIndex,str.begin()+found));
			startIndex=found+1;//�����¸����
			found=str.find_first_of(splitTag,startIndex);
		}
		res.push_back(string(str.begin()+startIndex,str.end()));//�������һ��

		return res;
	}

	_COMMONTOOLSINOUT int StrToInt( const string str )
	{
		stringstream ss(str);
		int res;
		ss>>res;
		return res;
	}

	_COMMONTOOLSINOUT void TrimBeginEndBlank( string& str )
	{
		CREATE_FUNCTOR_O(AppendNonBlank,char,string,
			if(input!=' ')
			{
				_output.push_back(input);
			}
		);

		AppendNonBlank app;
		app=for_each(str.begin(),str.end(),app);

		str=app.GetResult();
	}

	wstring AsciiToUnicode(const string& str) {
		// Ԥ��-�������п��ֽڵĳ���    
		int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
		// ��ָ�򻺳�����ָ����������ڴ�    
		wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
		// ��ʼ�򻺳���ת���ֽ�    
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
		wstring ret_str = pUnicode;
		free(pUnicode);
		return ret_str;
	}
	string UnicodeToAscii(const wstring& wstr) {
		// Ԥ��-�������ж��ֽڵĳ���    
		int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		// ��ָ�򻺳�����ָ����������ڴ�    
		char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
		// ��ʼ�򻺳���ת���ֽ�    
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
		string ret_str = pAssii;
		free(pAssii);
		return ret_str;
	}
	wstring Utf8ToUnicode(const string& str) {
		// Ԥ��-�������п��ֽڵĳ���    
		int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		// ��ָ�򻺳�����ָ����������ڴ�    
		wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
		// ��ʼ�򻺳���ת���ֽ�    
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);
		wstring ret_str = pUnicode;
		free(pUnicode);
		return ret_str;
	}
	string UnicodeToUtf8(const wstring& wstr) {
		// Ԥ��-�������ж��ֽڵĳ���    
		int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		// ��ָ�򻺳�����ָ����������ڴ�    
		char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
		// ��ʼ�򻺳���ת���ֽ�    
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
		string ret_str = pAssii;
		free(pAssii);
		return ret_str;
	}
	string AsciiToUtf8(const string& str) {
		return UnicodeToUtf8(AsciiToUnicode(str));
	}
	string Utf8ToAscii(const string& str) {
		return UnicodeToAscii(Utf8ToUnicode(str));
	}
}

