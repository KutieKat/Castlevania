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
