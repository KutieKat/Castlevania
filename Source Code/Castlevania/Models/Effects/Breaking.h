#pragma once
#include <vector>

#include "../GameObject.h"
#include "BrickPiece.h"
#include "../../Utilities/Debug.h"
using namespace std;

class CBreaking : public CGameObject
{
public:
	CBreaking(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();

protected:
	vector<CBrickPiece*> pieces;

	void GetPieceSpeed(int index, float& speedX, float &speedY);
};

