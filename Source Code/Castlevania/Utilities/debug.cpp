#include "Debug.h"
#include "Convert.h"

void CDebug::Info(string message, string fileName)
{
	OutputMessage("INFO", message, fileName);
}

void CDebug::Error(string message, string fileName)
{
	OutputMessage("ERROR", message, fileName);
}

void CDebug::OutputMessage(string tag, string message, string fileName)
{
	string outputMessage = "[" + tag + "]";

	if (fileName.empty())
	{
		outputMessage += " " + message;
	}
	else
	{
		outputMessage += "[" + fileName + "] " + message;
	}

	outputMessage += "\n";

	wstring wstr = wstring(outputMessage.begin(), outputMessage.end());

	OutputDebugString(wstr.c_str());
}
