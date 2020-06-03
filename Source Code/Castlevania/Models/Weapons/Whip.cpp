#include "Whip.h"
#include "../../Game.h"
#include "../Characters/Enemies/RedBat.h"
#include "../Characters/Enemies/SpearKnight.h"
#include "../Misc/BigCandle.h"
#include "../Misc/SmallCandle.h"
#include "../Misc/BreakableBrick.h"

CWhip::CWhip(CSimon* simon)
{
	this->simon = simon;

	SetAnimationSet("whip");
}

void CWhip::Render()
{
	animationSet->at(GetAnimationToRender())->Render(x, y);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();

	for (int i = 0; i < coObjects->size(); i++)
	{
		auto object = coObjects->at(i);

		if (HaveCollision(object))
		{
			if (dynamic_cast<CBigCandle*>(object))
			{
				object->Disappear();
			}
			else if (dynamic_cast<CSmallCandle*>(object))
			{
				object->Disappear();
			}
			else if (dynamic_cast<CBreakableBrick*>(object))
			{
				object->Disappear();
			}
			else if (dynamic_cast<CEnemy*>(object))
			{
				auto enemy = dynamic_cast<CEnemy*>(object);

				enemy->TakeDamage(playerData->GetPower());
			}
		}
	}
}

void CWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	b = y + WHIP_BBOX_HEIGHT;

	switch (CGame::GetInstance()->GetPlayerData()->GetWhipLevel())
	{
	case WHIP_LEVEL_1:
		r = l + WHIP_LEVEL_1_BBOX_WIDTH;
		break;

	case WHIP_LEVEL_2:
		r = l + WHIP_LEVEL_2_BBOX_WIDTH;
		break;

	case WHIP_LEVEL_3:
		r = l + WHIP_LEVEL_3_BBOX_WIDTH;
		break;
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

	if (dynamic_cast<CBreakableBrick*>(object))
	{
		wcy = wt + (wb - wt) / 3;
	}
	else
	{
		wcy = wt + (wb - wt) / 2;
	}

	obcx = obl + (obr - obl) / 2;

	if (CGame::GetInstance()->HaveCollision(this, object))
	{
		if (directionX == Direction::Right && wcy >= obt && scx < obcx && wr >= obl)
		{
			return true;
		}
		else if (directionX == Direction::Left && wcy >= obt && scx > obcx && wl <= obr)
		{
			return true;
		}
	}

	return false;
}

int CWhip::GetAnimationToRender()
{
	int ani;

	switch (CGame::GetInstance()->GetPlayerData()->GetWhipLevel())
	{
	case WHIP_LEVEL_1:
		ani = directionX == Direction::Right ? WHIP_ANI_LEVEL_1_RIGHT : WHIP_ANI_LEVEL_1_LEFT;
		break;

	case WHIP_LEVEL_2:
		ani = directionX == Direction::Right ? WHIP_ANI_LEVEL_2_RIGHT : WHIP_ANI_LEVEL_2_LEFT;
		break;

	case WHIP_LEVEL_3:
		ani = directionX == Direction::Right ? WHIP_ANI_LEVEL_3_RIGHT : WHIP_ANI_LEVEL_3_LEFT;
		break;
	}

	return ani;
}

void CWhip::ResetAnimations()
{
	CGameObject::ResetAnimations();
}