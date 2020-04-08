#include "TileMap.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Textures/TextureManager.h"
#include "../Sprites/SpriteManager.h"
#include "../Utilities/Color.h"
#include "../Utilities/Debug.h"

CTileMap::CTileMap(string mapFile, LPWSTR tilesetFile)
{
	this->mapFile = mapFile;
	this->tilesetFile = tilesetFile;

	LoadMap();
	LoadTiles();
}

void CTileMap::LoadMap()
{
	TiXmlDocument doc(mapFile.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* row = nullptr;

	root->QueryIntAttribute("tileWidth", &this->tileWidth);
	root->QueryIntAttribute("tileHeight", &this->tileHeight);
	root->QueryIntAttribute("tilesetWidth", &this->tilesetWidth);
	root->QueryIntAttribute("tilesetHeight", &this->tilesetHeight);

	int rowsCount = 0;
	int columnsCount = 0;

	for (row = root->FirstChildElement(); row != nullptr; row = row->NextSiblingElement())
	{
		vector<int> columns;
		TiXmlElement* column = nullptr;

		for (column = row->FirstChildElement(); column != nullptr; column = column->NextSiblingElement())
		{
			int value = stoi(column->GetText());

			columns.push_back(value);
		}

		if (columns.size() > columnsCount)
		{
			columnsCount = columns.size();
		}

		this->map.push_back(columns);

		rowsCount++;
	}

	this->width = tileWidth * columnsCount;
	this->height = tileHeight * rowsCount;
	this->mapRows = this->height / tileHeight;
	this->mapColumns = this->width / tileWidth;
	this->tilesetRows = this->tilesetHeight / tileHeight;
	this->tilesetColumns = this->tilesetWidth / tileWidth;
}

void CTileMap::LoadTiles()
{
	CTextureManager* textureManager = CTextureManager::GetInstance();

	textureManager->Add("tiles_texture", tilesetFile, CColor::FromRgb(255, 0, 255));
	LPDIRECT3DTEXTURE9 tiles = textureManager->Get("tiles_texture");

	int count = 0;

	for (int row = 0; row < this->tilesetRows; row++)
	{
		for (int column = 0; column < this->tilesetColumns; column++)
		{
			string tileId = "tile_" + to_string(count);

			int left = column * tileWidth;
			int top = row * tileHeight;
			int right = left + tileWidth;
			int bottom = top + tileHeight;

			CSpriteManager::GetInstance()->Add(tileId, left, top, right, bottom, tiles);

			count++;
		}
	}
}

void CTileMap::Render(float cameraLeft, float cameraTop, float cameraRight, float cameraBottom)
{
	int startingColumn = cameraLeft / tileWidth;
	int endingColumn = cameraRight / tileWidth;

	if (endingColumn >= this->mapColumns)
	{
		endingColumn--;
	}

	for (int row = 0; row < this->mapRows; row++)
	{
		for (int column = startingColumn; column <= endingColumn; column++)
		{
			string tileId = "tile_" + to_string(this->map[row][column]);

			float x = tileWidth * column;
			float y = tileHeight * row;

			CSpriteManager::GetInstance()->Get(tileId)->Draw(x, y);
		}
	}
}

int CTileMap::GetWidth()
{
	return this->width;
}

int CTileMap::GetHeight()
{
	return this->height;
}
