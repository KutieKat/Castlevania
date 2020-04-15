#pragma once
#include "Item.h"
#include "../../Utilities/Constants.h"

class CDagger : public CItem
{
public:
	CDagger();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};