#include "Bat.h"
#include "../../Utilities/Debug.h"

CBat::CBat()
{
	this->alpha = 0.0f;

	SetAnimationSet("bat");
}

void CBat::SetState(int state)
{
	CGameObject::SetState(state);
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (this->state == BAT_STATE_FLY_RIGHT_CROSS)
	{
		vx = vy = BAT_FLY_RIGHT_CROSS_SPEED;

		x += vx * dt;
		y -= vy * dt;
	}
	
	if (this->state == BAT_STATE_FLY_OVAL)
	{
		// Circular: https://answers.unity.com/questions/596671/circular-rotation-via-the-mathematical-circle-equa.html
		// Circular: https://gamedev.stackexchange.com/questions/67206/how-to-set-speed-of-circular-motion
		// Oval: https://gamedev.stackexchange.com/questions/128141/how-to-orbit-an-object-around-another-object-in-an-oval-path-in-unity
		// Deg2Rad: https://stackoverflow.com/questions/34583438/function-deg2rad-could-not-be-resolved

		x += 0.3f * sin(4.0f * atan(1.0f) / 180.0f * alpha);
		y += 0.1f * cos(4.0f * atan(1.0f) / 180.0f * alpha);

		alpha -= BAT_FLY_OVAL_SPEED * dt;
	}
}

void CBat::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}