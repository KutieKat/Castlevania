#pragma once
#include <d3dx9.h>
#include "../Sprites/Sprite.h"

class CAnimationFrame
{
public:
	CAnimationFrame(CSprite* sprite, int time);
	DWORD GetTime();
	CSprite* GetSprite();

	~CAnimationFrame();

private:
	CSprite* sprite;
	DWORD time;
};
