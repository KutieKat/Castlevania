#include "Whip.h"

CWhip::CWhip()
{
	level = 1;
}

void CWhip::Render()
{
	int ani;

	if (level == 1)
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
	else if (level == 2)
	{
		if (direction == Direction::Right)
		{
			ani = WHIP_ANI_LEVEL_2_RIGHT;
		}
		else
		{
			ani = WHIP_ANI_LEVEL_2_LEFT;
		}
	}
	else if (level == 3)
	{
		if (direction == Direction::Right)
		{
			ani = WHIP_ANI_LEVEL_3_RIGHT;
		}
		else
		{
			ani = WHIP_ANI_LEVEL_3_LEFT;
		}
	}

	animations[ani]->Render(x, y);
}

void CWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	b = y + WHIP_BBOX_HEIGHT;

	if (this->level == 1)
	{
		r = l + WHIP_LEVEL_1_BBOX_WIDTH;
	}
	else if (this->level == 2)
	{
		r = l + WHIP_LEVEL_2_BBOX_WIDTH;
	}
	else
	{
		r = l + WHIP_LEVEL_3_BBOX_WIDTH;
	}
}

void CWhip::Upgrade()
{
	if (this->level < 3)
	{
		this->level += 1;
	}
}
