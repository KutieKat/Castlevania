#include "Whip.h"

CWhip::CWhip()
{
	this->level = 1;
}

void CWhip::Render()
{
	animations[GetAnimationToRender()]->Render(x, y);
}

void CWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	b = y + WHIP_BBOX_HEIGHT;

	switch (this->level)
	{
	case 1:
		r = l + WHIP_LEVEL_1_BBOX_WIDTH;
		break;

	case 2:
		r = l + WHIP_LEVEL_2_BBOX_WIDTH;
		break;

	case 3:
		r = l + WHIP_LEVEL_3_BBOX_WIDTH;
		break;
	}
}

void CWhip::Upgrade()
{
	if (this->level < 3)
	{
		this->level += 1;
	}
}

int CWhip::GetAnimationToRender()
{
	int ani;

	switch (this->level)
	{
	case 1:
		ani = direction == Direction::Right ? WHIP_ANI_LEVEL_1_RIGHT : WHIP_ANI_LEVEL_1_LEFT;
		break;

	case 2:
		ani = direction == Direction::Right ? WHIP_ANI_LEVEL_2_RIGHT : WHIP_ANI_LEVEL_2_LEFT;
		break;

	case 3:
		ani = direction == Direction::Right ? WHIP_ANI_LEVEL_3_RIGHT : WHIP_ANI_LEVEL_3_LEFT;
		break;
	}

	return ani;
}