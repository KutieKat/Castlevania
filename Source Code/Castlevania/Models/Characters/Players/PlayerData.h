#pragma once
#include <string>
#include "../../../Utilities/Constants.h"
using namespace std;

class CPlayerData
{
public:
	static CPlayerData* GetInstance();

	void Init();
	void ResetPower();
	void ResetWeapons();
	void Reset();

	void AddScore(int score);
	int GetScore();

	void AddHearts(int hearts);
	void DecreaseHearts(int hearts);
	int GetHearts();

	void AddLives(int lives);
	void DecreaseLives();
	int GetLives();

	void AddHealthVolumes(int damages);
	void DecreaseHealthVolumes(int damages);
	void ResetHealthVolumes();
	int GetHealthVolumes();

	void SetSubWeaponType(string type);
	string GetSubWeaponType();

	void SetPower(int power);
	int GetPower();

	void SetWhipLevel(int level);
	int GetWhipLevel();

	void AddThrownSubWeapons();
	void DecreaseThrownSubWeapons();
	int GetThrownSubWeapons();

protected:
	int score;
	int hearts;
	int lives;
	int healthVolumes;
	int whipLevel;
	int power;
	int thrownSubWeapons;

	string subWeaponType;

	static CPlayerData* instance;
};

