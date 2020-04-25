#include "Whip.h"
#include "../../Game.h"
#include "../Misc/BigCandle.h"

CWhip::CWhip(CSimon* simon)
{
	this->simon = simon;
	this->level = CGame::GetInstance()->GetPlayerData()->GetWhipLevel();

	SetAnimationSet("whip");
}

void CWhip::Render()
{
	animationSet->at(GetAnimationToRender())->Render(x, y);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	for (int i = 0; i < coObjects->size(); i++)
	{
		auto object = coObjects->at(i);

		if (HaveCollision(object))
		{
			if (dynamic_cast<CBigCandle*>(object))
			{
				object->Disappear();
			}
		}
	}
}

void CWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	b = y + WHIP_BBOX_HEIGHT;

	switch (level)
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
	if (level < 3)
	{
		level += 1;
	}
}

bool CWhip::HaveCollision(CGameObject* object)
{
	float scx, sl, st, sr, sb;
	float wcy, wl, wt, wr, wb;
	float obcx, obl, obt, obr, obb;

	simon->GetBoundingBox(sl, st, sr, sb);
	object->GetBoundingBox(obl, obt, obr, obb);
	GetBoundingBox(wl, wt, wr, wb);

	scx = sl + (sr - sl) / 2;
	wcy = wt + (wb - wt) / 2;
	obcx = obl + (obr - obl) / 2;

	if (CGame::GetInstance()->HaveCollision(this, object))
	{
		if (direction == Direction::Right && wcy >= obt && scx < obcx && wr >= obl)
		{
			return true;
		}
		else if (direction == Direction::Left && wcy >= obt && scx > obcx && wl <= obr)
		{
			return true;
		}
	}

	return false;
}

int CWhip::GetLevel()
{
	return level;
}

int CWhip::GetAnimationToRender()
{
	int ani;

	switch (level)
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

void CWhip::ResetAnimations()
{
	CGameObject::ResetAnimations();
}