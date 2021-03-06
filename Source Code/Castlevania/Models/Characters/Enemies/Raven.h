#pragma once
#include "Enemy.h"
#include "../../../Utilities/Constants.h"

class CSimon;

class CRaven : public CEnemy
{
public:
	CRaven(CSimon* simon);

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void TakeDamage(int damages);
	void Attack();
	void OnPlayerEnterArea();

	int GetDamages();

protected:
	CSimon* simon;

	int flyingCounter;

	float angle;
	float targetX;
	float targetY;

	bool ReachedTarget();
	void GenerateTarget();
};