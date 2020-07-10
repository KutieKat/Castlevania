#include "SettingManager.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Utilities/Debug.h"

CSettingManager* CSettingManager::instance = nullptr;

bool CSettingManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Info(doc.ErrorDesc(), "SettingManager.cpp");
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* setting = nullptr;

	for (setting = root->FirstChildElement(); setting != nullptr; setting = setting->NextSiblingElement())
	{
		string id = setting->Attribute("id");
		string type = "";

		if (setting->Attribute("type"))
		{
			type = setting->Attribute("type");
		}

		if (type == "int")
		{
			int value;
			setting->QueryIntAttribute("value", &value);
			
			intSettings[id] = value;
		}
		else if (type == "float")
		{
			float value;
			setting->QueryFloatAttribute("value", &value);

			floatSettings[id] = value;
		}
		else if (type == "bool")
		{
			bool value;
			setting->QueryBoolAttribute("value", &value);

			boolSettings[id] = value;
		}
		else if (type == "string")
		{
			stringSettings[id] = setting->Attribute("value");
		}
		else
		{
			setting = setting->NextSiblingElement();
		}
	}

	return true;
}

int CSettingManager::GetIntValue(string id)
{
	return intSettings[id];
}

float CSettingManager::GetFloatValue(string id)
{
	return floatSettings[id];
}

bool CSettingManager::GetBoolValue(string id)
{
	return boolSettings[id];
}

string CSettingManager::GetStringValue(string id)
{
	return stringSettings[id];
}

CSettingManager* CSettingManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CSettingManager();
	}

	return instance;
}

CSettingManager::~CSettingManager()
{
}