#include "PlayerData.h"

CPlayerData* CPlayerData::instance = nullptr;

CPlayerData* CPlayerData::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CPlayerData();
	}

	return instance;
}

void CPlayerData::Init()
{
	Reset();
}

void CPlayerData::Reset()
{
	score = SIMON_DEFAULT_SCORE;
	hearts = SIMON_DEFAULT_HEARTS;
	lives = SIMON_DEFAULT_LIVES;
	healthVolumes = HEALTH_BAR_MAX_VOLUMES;
	whipLevel = WHIP_LEVEL_1;
	whipPower = WHIP_NORMAL_POWER;
	subWeaponType = "";
}

void CPlayerData::AddScore(int score)
{
	this->score += score;
}

int CPlayerData::GetScore()
{
	return score;
}

void CPlayerData::AddHearts(int hearts)
{
	this->hearts += hearts;
}

void CPlayerData::DecreaseHearts(int hearts)
{
	this->hearts -= hearts;
}

int CPlayerData::GetHearts()
{
	return hearts;
}

void CPlayerData::AddLives(int lives)
{
	this->lives += lives;
}

void CPlayerData::DecreaseLives()
{
	lives -= 1;
}

int CPlayerData::GetLives()
{
	return lives;
}

void CPlayerData::AddHealthVolumes(int volumes)
{
	healthVolumes += volumes;
}

void CPlayerData::DecreaseHealthVolumes(int volumes)
{
	healthVolumes -= volumes;
}

int CPlayerData::GetHealthVolumes()
{
	return healthVolumes;
}

void CPlayerData::SetSubWeaponType(string type)
{
	subWeaponType = type;
}

string CPlayerData::GetSubWeaponType()
{
	return subWeaponType;
}

void CPlayerData::SetWhipPower(int power)
{
	whipPower = power;
}

int CPlayerData::GetWhipPower()
{
	return whipPower;
}

void CPlayerData::SetWhipLevel(int level)
{
	if (level > 0 && level <= WHIP_LEVEL_3)
	{
		whipLevel = level;
	}
}

int CPlayerData::GetWhipLevel()
{
	return whipLevel;
}
