#include <d3dx9.h>
#include <algorithm>

#include "../Utilities/debug.h"
#include "../Textures/TextureManager.h"
#include "../Game.h"
#include "GameObject.h"
#include "../Sprites/SpriteManager.h"
#include "../Animations/AnimationSets.h"
#include "../Utilities/SafeDelete.h"
#include "../Models/Items/MagicCrystal.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	disappearingTime = -1;
	elevation = CSettingManager::GetInstance()->GetIntValue("DEFAULT_ELEVATION");

	directionX = Direction::Right;
	directionY = Direction::None;

	showingHiddenItem = true;
	showingEffect = false;
	showingEndingEffect = false;
	isEffect = false;
	isItem = false;
}

void CGameObject::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CGameObject::SetSpeed(float vx, float vy)
{
	this->vx = vx;
	this->vy = vy;
}

void CGameObject::GetPosition(float & x, float & y)
{
	x = this->x;
	y = this->y;
}

void CGameObject::GetSpeed(float & vx, float & vy)
{
	vx = this->vx;
	vy = this->vy;
}

void CGameObject::Pause()
{
	for (int i = 0; i < animationSet->size(); i++)
	{
		animationSet->at(i)->Pause();
	}
}

void CGameObject::SetDirectionX(Direction direction)
{
	this->directionX = direction;
}

Direction CGameObject::GetDirectionX()
{
	return directionX;
}

void CGameObject::SetDirectionY(Direction direction)
{
	directionY = direction;
}

Direction CGameObject::GetDirectionY()
{
	return directionY;
}

void CGameObject::SetId(string id)
{
	this->id = id;
}

string CGameObject::GetId()
{
	return id;
}

void CGameObject::SetEndingEffect(CGameObject* effect)
{
	endingEffect = effect;
	endingEffect->visibility = Visibility::Hidden;
}

void CGameObject::SetElevation(int elevation)
{
	this->elevation = elevation;
}

bool CGameObject::Over()
{
	return disappearingTime != -1 && GetTickCount() > disappearingTime;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;

	dx = vx * dt;
	dy = vy * dt;

	if (!isEffect && disappearingTime != -1 && GetTickCount() > CGame::GetInstance()->GetPauseDeltaTime() + disappearingTime)
	{
		disappearingTime = -1;
		removable = true;
	}

	if (endingEffect && endingEffect->Over())
	{
		if (showingEffect)
		{
			showingEffect = false;

			endingEffect->disappearingTime = -1;
			endingEffect->visibility = Visibility::Hidden;
		}
		
		if (showingEndingEffect)
		{
			if (hiddenItem && showingHiddenItem)
			{
				hiddenItem->SetVisibility(Visibility::Visible);
				hiddenItem->SetPosition(x, y);

				if (dynamic_cast<CMagicCrystal*>(hiddenItem))
				{
					// Do nothing
				}
				else
				{
					hiddenItem->SetDisplayTime(CSettingManager::GetInstance()->GetIntValue("ITEM_DISPLAY_TIME"));
				}
			}

			endingEffect->removable = true;
			removable = true;
		}
	}
}

void CGameObject::SetState(int state)
{
	this->state = state;
}

void CGameObject::ResetAnimations()
{
	for (int i = 0; i < animationSet->size(); i++)
	{
		animationSet->at(i)->Reset();
	}
}

void CGameObject::Disappear()
{
	if (endingEffect)
	{
		float ol, ot, or , ob;
		GetBoundingBox(ol, ot, or , ob);

		float el, et, er, eb;
		endingEffect->GetBoundingBox(el, et, er, eb);

		showingEndingEffect = true;

		endingEffect->SetVisibility(Visibility::Visible);
		endingEffect->SetPosition(ol + ((or - ol) - (er - el)) / 2 - 10, ot + ((ob - ot) - (eb - et)) / 2 - 10);
		endingEffect->SetDisplayTime(CSettingManager::GetInstance()->GetIntValue("ENDING_EFFECT_DISPLAY_TIME"));
	}
	else
	{
		removable = true;
	}
}

void CGameObject::ShowEffect()
{
	if (endingEffect)
	{
		float ol, ot, or , ob;
		GetBoundingBox(ol, ot, or , ob);

		float el, et, er, eb;
		endingEffect->GetBoundingBox(el, et, er, eb);

		showingEffect = true;

		endingEffect->SetVisibility(Visibility::Visible);
		endingEffect->SetPosition(ol + ((or -ol) - (er - el)) / 2 - 10, ot + ((ob - ot) - (eb - et)) / 2 - 10);
		endingEffect->SetDisplayTime(CSettingManager::GetInstance()->GetIntValue("EFFECT_DISPLAY_TIME"));
	}
}

void CGameObject::ShowHiddenItem()
{
	hiddenItem->SetVisibility(Visibility::Visible);
}

void CGameObject::HideHiddenItem()
{
	showingHiddenItem = false;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = dx - sdx;
	float rdy = dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty,
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

bool CGameObject::CompareElevation(CGameObject * a, CGameObject * b)
{
	return a->GetElevation() < b->GetElevation();
}

void CGameObject::SetVisibility(Visibility visibility)
{
	this->visibility = visibility;
}

Visibility CGameObject::GetVisibility()
{
	return visibility;
}

int CGameObject::GetState()
{
	return state;
}

int CGameObject::GetElevation()
{
	return elevation;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextureManager::GetInstance()->Get("bounding_box");

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 200);
}

void CGameObject::SetLeftBound(int left)
{
	leftBound = left;
}

void CGameObject::SetTopBound(int top)
{
	topBound = top;
}

void CGameObject::SetRightBound(int right)
{
	rightBound = right;
}

void CGameObject::SetBottomBound(int bottom)
{
	bottomBound = bottom;
}

void CGameObject::SetAnimationSet(string animationSetId)
{
	animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
}

void CGameObject::SetDisplayTime(DWORD time)
{
	disappearingTime = GetTickCount() + time;
}

void CGameObject::SetHiddenItem(CGameObject* item)
{
	hiddenItem = item;
	hiddenItem->SetVisibility(Visibility::Hidden);
}

CGameObject* CGameObject::GetHiddenItem()
{
	return hiddenItem;
}

CGameObject::~CGameObject()
{
}