#include <string>
#include <unordered_map>
using namespace std;

class CSettingManager
{
public:
	bool LoadFromFile(string filePath);

	int GetIntValue(string id);
	float GetFloatValue(string id);
	bool GetBoolValue(string id);
	string GetStringValue(string id);

	static CSettingManager* GetInstance();

	~CSettingManager();

protected:
	static CSettingManager* instance;

	unordered_map<string, int> intSettings;
	unordered_map<string, float> floatSettings;
	unordered_map<string, bool> boolSettings;
	unordered_map<string, string> stringSettings;
};