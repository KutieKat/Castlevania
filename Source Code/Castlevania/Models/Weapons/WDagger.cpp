#include "WDagger.h"
#include "../../Game.h"

WDagger::WDagger()
{
	SetAnimationSet("dagger");
}

void WDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (x > CGame::GetInstance()->GetCamera()->GetRight())
	{
		SetVisibility(Visibility::Hidden);
	}

	x += (direction == Direction::Right ? DAGGER_MOVE_SPEED : -DAGGER_MOVE_SPEED) * dt;
}

void WDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void WDagger::Render()
{
	int ani = direction == Direction::Right ? DAGGER_ANI_RIGHT : DAGGER_ANI_LEFT;

	animationSet->at(ani)->Render(x, y);
}
