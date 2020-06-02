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
	void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);
	void OnPlayerEnterArea();
	void Attack();

protected:
	CSimon* simon;

	DWORD nextAttackingTime;

	int jumpingCounter;
};