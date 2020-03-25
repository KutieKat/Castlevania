#pragma once
#include <string>
#include <d3d9.h>
using namespace std;

class CConvert
{
public:
	static string lpcwstrToString(LPCWSTR value);
	static LPCWSTR stringToLpcwstr(string value);
};

