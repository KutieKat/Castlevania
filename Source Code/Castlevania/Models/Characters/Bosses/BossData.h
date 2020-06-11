#pragma once
#include <string>
#include "../../../Utilities/Constants.h"
using namespace std;

class CBossData
{
public:
	static CBossData* GetInstance();

	void Init();
	void Reset();

	void DecreaseHealthVolumes(int damages = BOSS_DEFAULT_DAMAGES);
	int GetHealthVolumes();

protected:
	int healthVolumes;

	static CBossData* instance;
};