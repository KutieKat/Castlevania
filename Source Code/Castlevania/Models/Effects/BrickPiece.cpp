#include "BrickPiece.h"

CBrickPiece::CBrickPiece()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("brick_piece");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += vx * dt;
	y += vy * dt;
}

void CBrickPiece::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CBrickPiece::Render()
{
	animationSet->at(0)->Render(x, y);
}
