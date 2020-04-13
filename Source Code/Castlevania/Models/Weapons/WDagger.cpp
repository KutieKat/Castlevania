#include "WDagger.h"
#include "../../Game.h"

void WDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (x > CGame::GetInstance()->GetCamera()->GetRight())
	{
		this->SetVisibility(Visibility::Hidden);
	}

	vx += (direction == Direction::Right ? DAGGER_MOVE_SPEED : -DAGGER_MOVE_SPEED) * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		//x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		x += dx;
		y += dy;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
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

	animations[ani]->Render(x, y);
}
