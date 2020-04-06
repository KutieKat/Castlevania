#pragma once
#include "Item.h"
#include "../../Utilities/Constants.h"

class CDagger : public CItem
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};