#include "Convert.h"

string CConvert::lpcwstrToString(LPCWSTR value)
{
	wstring wstr(value);

	return string(wstr.begin(), wstr.end());
}

LPCWSTR CConvert::stringToLpcwstr(string value)
{
	wstring wstr = wstring(value.begin(), value.end());

	return wstr.c_str();
}

wstring CConvert::s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}
