#include "BigDoubleShot.h"
#include "../../Game.h"

CBigDoubleShot::CBigDoubleShot(bool blinking)
{
	SetAnimationSet("big_double_shot");
	animationSet->at(BIG_DOUBLE_SHOT_ANI_BLINKING)->SetStartTime(GetTickCount());

	this->blinking = blinking;
}

void CBigDoubleShot::Render()
{
	int ani;

	if (blinking)
	{
		if (animationSet->at(BIG_DOUBLE_SHOT_ANI_BLINKING)->Over())
		{
			ani = BIG_DOUBLE_SHOT_ANI_STATIC;
		}
		else
		{
			ani = BIG_DOUBLE_SHOT_ANI_BLINKING;
		}
	}
	else
	{
		ani = BIG_DOUBLE_SHOT_ANI_STATIC;
	}

	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(ani)->Render(x, y);
}

void CBigDoubleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}