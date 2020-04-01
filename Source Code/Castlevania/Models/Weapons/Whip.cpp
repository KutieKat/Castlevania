#include "Whip.h"
#include "../Misc/BigCandle.h"

CWhip::CWhip()
{
	level = 1;
}

void CWhip::Render()
{
	int ani;

	if (level == WHIP_LEVEL_1)
	{
		if (direction == Direction::Right)
		{
			ani = WHIP_ANI_LEVEL_1_RIGHT;
		}
		else
		{
			ani = WHIP_ANI_LEVEL_1_LEFT;
		}
	}

	animations[ani]->Render(x, y);
}

void CWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + WHIP_BBOX_WIDTH;
	b = y + WHIP_BBOX_HEIGHT;
}

CBoundingBox CWhip::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}

void CWhip::SetLevel(int level)
{
	this->level = level;
}

void CWhip::ResetAnimations()
{
	for (int i = 0; i < animations.size(); i++)
	{
		animations[i]->Reset();
	}
}
