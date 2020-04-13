#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
#include "../../Weapons/Weapon.h"
#include "../../Items/Item.h"

class CSimon : public CGameObject
{
public:
	CSimon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ResetAnimations();

	int GetAnimationToRender();

	bool Sitting();
	bool TouchingGround();
	bool Up();

	void AddScore(int addedScore);
	int GetScore();

	void AddHeart(int hearts);
	void DecreaseHeart(int hearts);
	int GetHearts();

	void AddLife(int lives);
	int GetLives();

	void SetSubWeapon(CWeapon* weapon);
	CWeapon* GetSubWeapon();

	void TakeDamage(int volumes = 1);
	int getHealthVolumes();

	void SetSubWeaponType(string type);
	string GetSubWeaponType();

	void SetUp(bool up);

	~CSimon();

protected:
	CWeapon* whip;
	CWeapon* subWeapon;

	bool sitting;
	bool touchingGround;
	bool up;

	DWORD delayEndTime;

	int score;
	int hearts;
	int lives;
	int healthVolumes;

	string subWeaponType;

	void InitSubWeapon();
	void UpdateWhip();
};