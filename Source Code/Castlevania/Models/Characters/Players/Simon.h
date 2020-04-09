#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
#include "../../Weapons/Weapon.h"
#include "../../Items/Item.h"

class CSimon : public CGameObject
{
public:
	CSimon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ResetAnimations();

	int GetAnimationToRender();

	bool Sitting();
	bool TouchingGround();

	void AddScore(int addedScore);
	int GetScore();

	void AddHeart();
	int GetHearts();

	void AddLife();
	int GetLives();

	void SetSubWeapon(CWeapon* weapon);
	CWeapon* GetSubWeapon();

	void TakeDamage(int volumes = 1);
	int getHealthVolumes();

	void SetSubWeaponType(string type);
	string GetSubWeaponType();

protected:
	CWeapon* whip;
	CWeapon* subWeapon;

	bool sitting;
	bool touchingGround;

	DWORD delayEndTime;

	int score;
	int hearts;
	int lives;
	int healthVolumes;

	string subWeaponType;
};