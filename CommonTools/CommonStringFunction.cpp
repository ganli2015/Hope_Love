#include "StdAfx.h"
#include "CommonStringFunction.h"

#include "GeneralFunctor.h"

#include "windows.h"  

#include <functional>

#include <hashlibpp.h>

namespace CommonTool
{
	vector<string> SplitString( const string& str, const char splitTag )
	{
		vector<string> res;
		int startIndex=0;
		size_t found=str.find_first_of(splitTag,startIndex);//查找分割点的序号
		while(found!=string::npos)
		{
			res.push_back(string(str.begin()+startIndex,str.begin()+found));
			startIndex=found+1;//更新下个起点
			found=str.find_first_of(splitTag,startIndex);
		}
		res.push_back(string(str.begin()+startIndex,str.end()));//弹入最后一段

		return res;
	}

	void RemoveEmptyString(vector<string>& vec)
	{
		for (vector<string>::iterator it=vec.begin();it!=vec.end();)
		{
			if (*it == "")
			{
				it = vec.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	bool StartWith(const string str, const char tag)
	{
		return find(str.begin(), str.end(), tag) == str.begin();
	}

	string::const_iterator FindChar(const string& str, const char tag)
	{
		return find(str.begin(), str.end(), tag);
	}

	string::const_iterator FindString(const string& str, const string tag)
	{
		return FindString(str, str.cbegin(), tag);
	}

	string::const_iterator FindString(const string& str, const string::const_iterator& startPos, const string tag)
	{
		size_t off = distance(str.begin(), startPos);
		auto index = str.find(tag,off);
		if (index == string::npos)
		{
			return str.cend();
		}
		else
		{
			return str.begin() + index;
		}
	}

	bool HasString(const string& str, const string& target)
	{
		return FindString(str, target) != str.cend();
	}

	_COMMONTOOLSINOUT int StrToInt(const string str)
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
		// 预算-缓冲区中宽字节的长度    
		int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
		// 给指向缓冲区的指针变量分配内存    
		wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
		// 开始向缓冲区转换字节    
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
		wstring ret_str = pUnicode;
		free(pUnicode);
		return ret_str;
	}
	string UnicodeToAscii(const wstring& wstr) {
		// 预算-缓冲区中多字节的长度    
		int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		// 给指向缓冲区的指针变量分配内存    
		char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
		// 开始向缓冲区转换字节    
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
		string ret_str = pAssii;
		free(pAssii);
		return ret_str;
	}
	wstring Utf8ToUnicode(const string& str) {
		// 预算-缓冲区中宽字节的长度    
		int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		// 给指向缓冲区的指针变量分配内存    
		wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
		// 开始向缓冲区转换字节    
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);
		wstring ret_str = pUnicode;
		free(pUnicode);
		return ret_str;
	}
	string UnicodeToUtf8(const wstring& wstr) {
		// 预算-缓冲区中多字节的长度    
		int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		// 给指向缓冲区的指针变量分配内存    
		char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
		// 开始向缓冲区转换字节    
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

	std::string Getline_UnicodeFile(ifstream& fin, size_t& index)
	{
		wstring wstrLine = L"";
		string strLineAnsi = "";

		while (true && !fin.eof())
		{
			fin.seekg(index, ios::beg);
			wchar_t wch;
			fin.read((char *)(&wch), 2);
			if (wch == 0x000D) // 判断回车  
			{
				strLineAnsi = UnicodeToAscii(wstrLine);
				wstrLine.erase(0, wstrLine.size() + 1);
				index += 4; // 跳过回车符和行开头符  

				break;
			}
			else
			{
				wstrLine.append(1, wch);
				index += 2;
			}
		}

		return strLineAnsi;
	}


	size_t GetStrHash(const string str)
	{
		hash<string> hs;
		return hs(str);
	}

	char* StringFormat(const char * format, ...)
	{
		va_list args;
		va_start(args, format);

		const int bufferSize = 4096;
		char buff[bufferSize];
		int result = vsnprintf(buff, bufferSize - 1, format, args);

		va_end(args);

		return buff;
	}

	string GenerateHash(const string str)
	{
		hashwrapper *myWrapper = new md5wrapper();
		string res = myWrapper->getHashFromString(str);

		delete myWrapper;
		myWrapper = NULL;

		return res;
	}

}

