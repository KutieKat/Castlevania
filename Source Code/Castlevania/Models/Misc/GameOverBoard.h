#pragma once
#include "../GameObject.h"
#include "Cursor.h"
#include "../../Utilities/Constants.h"
#include "../../Utilities/Label.h"

class CGameOverBoard : public CGameObject
{
public:
	CGameOverBoard();

	virtual void Update();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void Up();
	void Down();
	void Select();

protected:
	CLabel* gameOverLabel;
	CLabel* continueLabel;
	CLabel* endLabel;

	CCursor* cursor;

	int index;
};