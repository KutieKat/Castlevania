#pragma once
#include "Enemy.h"
#include "../Players/Simon.h"
#include "../../../Utilities/Constants.h"

class CWhiteSkeleton : public CEnemy
{
public:
	CWhiteSkeleton(CSimon* simon);

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);
	void OnPlayerEnterArea();
	void Attack();
	void Disappear();

protected:
	CSimon* simon;

	bool touchingGround;

	int jumpingCounter;
	int attackingCounter;
	int walkingCounter;
	int GetAnimationToRender();
	int RandomizeInterval();
};