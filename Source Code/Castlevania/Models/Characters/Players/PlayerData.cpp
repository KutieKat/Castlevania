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

void CPlayerData::ResetWeapons()
{
	whipLevel = WHIP_LEVEL_1;
	subWeaponType = "";
}

void CPlayerData::Reset()
{
	score = SIMON_DEFAULT_SCORE;
	hearts = SIMON_DEFAULT_HEARTS;
	lives = SIMON_DEFAULT_LIVES;
	healthVolumes = HEALTH_BAR_MAX_VOLUMES;
	whipLevel = WHIP_LEVEL_1;
	power = NORMAL_POWER;
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

void CPlayerData::AddHealthVolumes(int damages)
{
	healthVolumes += damages;
}

void CPlayerData::DecreaseHealthVolumes(int damages)
{
	healthVolumes -= damages;
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

void CPlayerData::SetPower(int power)
{
	this->power = power;
}

int CPlayerData::GetPower()
{
	return power;
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

void CPlayerData::AddThrownSubWeapons()
{
	thrownSubWeapons += 1;
}

void CPlayerData::DecreaseThrownSubWeapons()
{
	if (thrownSubWeapons > 0)
	{
		thrownSubWeapons -= 1;
	}
}

int CPlayerData::GetThrownSubWeapons()
{
	return thrownSubWeapons;
}
