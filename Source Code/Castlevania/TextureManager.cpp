#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "TextureManager.h"

CTextureManager* CTextureManager::instance = nullptr;

CTextureManager* CTextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CTextureManager();
	}

	return instance;
}

void CTextureManager::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	if (this->textures.find(id) == this->textures.end())
	{
		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
		if (result != D3D_OK)
		{
			//DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
			return;
		}

		LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
		LPDIRECT3DTEXTURE9 texture;

		result = D3DXCreateTextureFromFileEx(
			d3ddv,								// Pointer to Direct3D device object
			filePath,							// Path to the image to load
			info.Width,							// Texture width
			info.Height,						// Texture height
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			transparentColor,
			&info,
			NULL,
			&texture);								// Created texture pointer

		if (result != D3D_OK)
		{
			//OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
			return;
		}

		this->textures[id] = texture;

		//DebugOut(L"[INFO] Texture loaded OK: id=%d, %s \n", id, filePath);
	}

}

LPDIRECT3DTEXTURE9 CTextureManager::Get(string id)
{
	return textures[id];
}