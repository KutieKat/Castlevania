#pragma once
#include <d3dx9.h>
#include "Sprite.h"

class CAnimationFrame
{
public:
	CAnimationFrame(CSprite* sprite, int time);
	DWORD GetTime();
	CSprite* GetSprite();

private:
	CSprite* sprite;
	DWORD time;
};
