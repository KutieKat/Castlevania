#include "BigTripleShot.h"

CBigTripleShot::CBigTripleShot(bool blinking)
{
	SetAnimationSet("big_triple_shot");
	animationSet->at(BIG_TRIPLE_SHOT_ANI_BLINKING)->SetStartTime(GetTickCount());

	this->blinking = blinking;
}

void CBigTripleShot::Render()
{
	int ani;

	if (blinking)
	{
		if (animationSet->at(BIG_TRIPLE_SHOT_ANI_BLINKING)->Over())
		{
			ani = BIG_TRIPLE_SHOT_ANI_STATIC;
		}
		else
		{
			ani = BIG_TRIPLE_SHOT_ANI_BLINKING;
		}
	}
	else
	{
		ani = BIG_TRIPLE_SHOT_ANI_STATIC;
	}

	animationSet->at(ani)->Render(x, y);
}

void CBigTripleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}