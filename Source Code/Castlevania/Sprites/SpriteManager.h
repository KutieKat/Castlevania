#pragma once
#include "Sprite.h"
#include <unordered_map>
#include <string>
using namespace std;

class CSpriteManager
{
public:
	bool LoadFromFile(string filePath);
	void Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);
	CSprite* Get(string id);
	CSprite* &operator[](string id);

	static CSpriteManager* GetInstance();

private:
	static CSpriteManager* instance;

	unordered_map<string, CSprite*> sprites;
};

