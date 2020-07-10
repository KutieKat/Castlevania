#include "BossData.h"
#include "../../../Game.h"

CBossData* CBossData::instance = nullptr;

CBossData* CBossData::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CBossData();
	}

	return instance;
}

void CBossData::Init()
{
	Reset();
}

void CBossData::Reset()
{
	healthVolumes = CSettingManager::GetInstance()->GetIntValue("BOSS_DEFAULT_HEALTHS");
}

void CBossData::DecreaseHealthVolumes(int damages)
{
	healthVolumes -= damages;
}

int CBossData::GetHealthVolumes()
{
	return healthVolumes;
}