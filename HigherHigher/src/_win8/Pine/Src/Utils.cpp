#include "pch.h"
#include <..\um\debugapi.h>

#include <stdarg.h>//#include <cstdio>
#include <Windows.h>
char* Utils:: ConvertStringToCharArray(Platform::String^ string)
{
	if (string != nullptr)
	{
		const wchar_t *W = string->Data();

		int Size = wcslen(W);
		char *CString = new char[Size + 1];
		CString[Size] = 0;
		int offset = 0;
		for (int y = 0; y<Size; y++)
		{
			if ((W[y] >= 32 && W[y] <= 126)){
				CString[offset] = (char)W[y];
				offset++;
			}
			else{
				CString[offset] = '!';
				offset++;
			}
		}
		CString[offset] = '\0';
		return CString;
	}
	return NULL;
}
Platform::String^ Utils::ConvertCharToString(const char* string)
{

	std::string s_str = std::string(string);
	std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
	const wchar_t* w_char = wid_str.c_str();
	Platform::String^ p_string = ref new Platform::String(w_char);
	return p_string;
}
#if defined(_DEBUG)
extern "C" void win8_log(const char* format, ...){
	char buf[2048];
	va_list ptr;
	va_start(ptr, format);
	vsprintf(buf, format, ptr);
	OutputDebugStringA(buf);
}
#endif