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
	score = 0;
	hearts = 5;
	lives = 3;
	healthVolumes = 16;
	whipLevel = 1;
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

void CPlayerData::SetWhipLevel(int level)
{
	whipLevel = level;
}

int CPlayerData::GetWhipLevel()
{
	return whipLevel;
}
