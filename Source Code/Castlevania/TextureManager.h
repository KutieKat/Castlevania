#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <string>
using namespace std;

class CTextureManager
{
public: 
	void Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(string id);

	static CTextureManager* GetInstance();

private:
	static CTextureManager* instance;

	unordered_map<string, LPDIRECT3DTEXTURE9> textures;
};