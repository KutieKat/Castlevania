#include "PlayerData.h"
#include "../../../Game.h"

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

void CPlayerData::ResetPower()
{
	power = CGame::GetInstance()->GetSettingManager()->GetIntValue("SIMON_DEFAULT_POWER");
}

void CPlayerData::ResetWeapons()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	whipLevel = settingManager->GetIntValue("SIMON_DEFAULT_WHIP_LEVEL");
	subWeaponType = settingManager->GetStringValue("SIMON_DEFAULT_SUB_WEAPON_TYPE");
}

void CPlayerData::Reset()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	score = settingManager->GetIntValue("SIMON_DEFAULT_SCORE");
	hearts = settingManager->GetIntValue("SIMON_DEFAULT_HEARTS");
	lives = settingManager->GetIntValue("SIMON_DEFAULT_LIVES");
	healthVolumes = settingManager->GetIntValue("SIMON_DEFAULT_HEALTHS");
	whipLevel = settingManager->GetIntValue("SIMON_DEFAULT_WHIP_LEVEL");
	power = settingManager->GetIntValue("SIMON_DEFAULT_POWER");
	subWeaponType = settingManager->GetStringValue("SIMON_DEFAULT_SUB_WEAPON_TYPE");
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

void CPlayerData::ResetHealthVolumes()
{
	healthVolumes = CGame::GetInstance()->GetSettingManager()->GetIntValue("SIMON_DEFAULT_HEALTHS");
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
