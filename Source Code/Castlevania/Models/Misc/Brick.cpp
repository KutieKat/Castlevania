#include "Brick.h"

void CBrick::Render()
{
	animations[0]->Render(x, y);
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BRICK_BBOX_WIDTH;
	bottom = top + BRICK_BBOX_HEIGHT;
}