#include "BossData.h"

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
	healthVolumes = HEALTH_BAR_MAX_VOLUMES;
}

void CBossData::DecreaseHealthVolumes(int damages)
{
	healthVolumes -= damages;
}

int CBossData::GetHealthVolumes()
{
	return healthVolumes;
}