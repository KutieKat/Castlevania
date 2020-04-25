#pragma once
#include <string>
using namespace std;

class CPlayerData
{
public:
	static CPlayerData* GetInstance();

	void Init();
	void Reset();

	void AddScore(int score);
	int GetScore();

	void AddHearts(int hearts);
	void DecreaseHearts(int hearts);
	int GetHearts();

	void AddLives(int lives);
	void DecreaseLives();
	int GetLives();

	void AddHealthVolumes(int volumes);
	void DecreaseHealthVolumes(int volumes);
	int GetHealthVolumes();

	void SetSubWeaponType(string type);
	string GetSubWeaponType();

	void SetWhipLevel(int level);
	int GetWhipLevel();

protected:
	int score;
	int hearts;
	int lives;
	int healthVolumes;
	int whipLevel;

	string subWeaponType;

	static CPlayerData* instance;
};

