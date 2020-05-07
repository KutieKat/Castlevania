#pragma once
#include "../GameObject.h"

class CBrickPiece: public CGameObject
{
public:
	CBrickPiece();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

