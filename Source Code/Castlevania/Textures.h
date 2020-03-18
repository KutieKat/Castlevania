#pragma once
#include <unordered_map>
#include <d3dx9.h>
using namespace std;

class CTextures
{
public: 
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(int i);

	static CTextures* GetInstance();

private:
	static CTextures* instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;
};