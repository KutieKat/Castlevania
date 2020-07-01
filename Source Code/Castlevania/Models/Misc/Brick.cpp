#include "Brick.h"

CBrick::CBrick()
{
	//isGround = false;
	canJumpThrough = false;
	SetAnimationSet("transparency");
}

void CBrick::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CBrick::SetWidth(float width)
{
	this->width = width;
}

void CBrick::SetHeight(float height)
{
	this->height = height;
}
