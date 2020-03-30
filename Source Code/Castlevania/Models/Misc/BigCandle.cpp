#include "BigCandle.h"

void CBigCandle::Render()
{
	animations[0]->Render(x, y);
}

void CBigCandle::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + BIG_CANDLE_BBOX_WIDTH;
	b = y + BIG_CANDLE_BBOX_HEIGHT;
}

CBoundingBox CBigCandle::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}
