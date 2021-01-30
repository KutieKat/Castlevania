#pragma once
#include "Enemy.h"
#include "../Players/Simon.h"
#include "../../../Utilities/Constants.h"

class CFleamen : public CEnemy
{
public:
	CFleamen(CSimon* simon);

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	void TakeDamage(int damages);
	void OnPlayerEnterArea();
	void Attack();

	int GetDamages();

protected:
	CSimon* simon;

	DWORD nextAttackingTime;

	int jumpingCounter;
};