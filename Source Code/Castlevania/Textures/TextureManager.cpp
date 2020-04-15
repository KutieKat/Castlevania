#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"
#include "../Game.h"
#include "../Textures/TextureManager.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Utilities/Color.h"
#include "../Utilities/Convert.h"

CTextureManager* CTextureManager::instance = nullptr;

CTextureManager* CTextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CTextureManager();
	}

	return instance;
}

bool CTextureManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* texture = nullptr;

	for (texture = root->FirstChildElement(); texture != nullptr; texture = texture->NextSiblingElement())
	{
		int r, g, b;

		string id = texture->Attribute("id");

		string path = texture->Attribute("path");
		wstring texturePath(path.begin(), path.end());

		texture->QueryIntAttribute("r", &r);
		texture->QueryIntAttribute("g", &g);
		texture->QueryIntAttribute("b", &b);

		Add(id, texturePath.c_str(), CColor::FromRgb(r, g, b));
	}

	return true;
}

void CTextureManager::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	if (this->textures.find(id) == this->textures.end())
	{
		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
		string path = CConvert::lpcwstrToString(filePath);

		if (result != D3D_OK)
		{
			CDebug::Error("GetImageInfoFromFile failed: " + path, "TextureManager.cpp");
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
			CDebug::Error("CreateTextureFromFile failed!", "TextureManager.cpp");
			return;
		}

		this->textures[id] = texture;

		CDebug::Info("Texture loaded successfully: ID=" + id + ", Path=" + path, "TextureManager.cpp");
	}
}

LPDIRECT3DTEXTURE9 CTextureManager::Get(string id)
{
	return textures[id];
}

void CTextureManager::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;

		if (tex != NULL)
		{
			tex->Release();
		}
	}

	textures.clear();
}

CTextureManager::~CTextureManager()
{
	Clear();

	SAFE_DELETE(instance);
}