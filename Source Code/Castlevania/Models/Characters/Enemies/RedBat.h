#pragma once
#include "Enemy.h"
#include "../../../Utilities/Constants.h"

class CRedBat : public CEnemy
{
public:
	CRedBat();

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);
	void OnPlayerEnterArea();
};