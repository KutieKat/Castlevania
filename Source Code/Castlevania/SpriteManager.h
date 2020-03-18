#pragma once
#include "Sprite.h"
#include <unordered_map>
using namespace std;

class CSpriteManager
{
public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);
	CSprite* Get(int id);
	CSprite* &operator[](int id);

	static CSpriteManager* GetInstance();

private:
	static CSpriteManager* instance;

	unordered_map<int, CSprite*> sprites;
};

