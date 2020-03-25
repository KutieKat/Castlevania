#pragma once

#include <string>
#include <Windows.h>
using namespace std;

class CDebug
{
public:
	static void Info(string message, string fileName = "");
	static void Error(string message, string fileName = "");

private:
	static void OutputMessage(string tag, string message, string fileName = "");
};

