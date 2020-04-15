#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <string>
#include "../Utilities/Debug.h"
using namespace std;

class CTextureManager
{
public:
	bool LoadFromFile(string filePath);
	void Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(string id);
	void Clear();

	static CTextureManager* GetInstance();
	void Show() { CDebug::Info("Length=" + to_string(textures.size())); }

	~CTextureManager();

private:
	static CTextureManager* instance;

	unordered_map<string, LPDIRECT3DTEXTURE9> textures;
};