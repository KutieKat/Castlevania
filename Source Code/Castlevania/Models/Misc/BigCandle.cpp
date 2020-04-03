#include "BigCandle.h"

void CBigCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->started && this->startingEffect->Over())
	{
		this->started = false;
		this->startingEffect->Reset();
	}

	if (this->ended && this->endingEffect->Over())
	{
		this->visibility = Visibility::Hidden;
		this->endingEffect->Reset();
	}
}

void CBigCandle::Render()
{
	if (this->started)
	{
		this->startingEffect->Start();
		this->startingEffect->SetPosition(x, y);
		this->startingEffect->Render();
	}
	else if (this->ended)
	{
		this->endingEffect->Start();
		this->endingEffect->SetPosition(x, y);
		this->endingEffect->Render();
	}
	else
	{
		animations[0]->Render(x, y);
	}
}

void CBigCandle::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!this->started && !this->ended)
	{
		l = x;
		t = y;
		r = x + BIG_CANDLE_BBOX_WIDTH;
		b = y + BIG_CANDLE_BBOX_HEIGHT;
	}
}

CBoundingBox CBigCandle::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}
