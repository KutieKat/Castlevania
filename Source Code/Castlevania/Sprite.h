#pragma once
#include <d3dx9.h>
#include <string>
using namespace std;

class CSprite
{
public:
	CSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);

	void Draw(float x, float y, int alpha = 255);

private:
	string id;

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
};
