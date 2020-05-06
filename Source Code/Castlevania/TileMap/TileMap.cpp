#include "TileMap.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Textures/TextureManager.h"
#include "../Sprites/SpriteManager.h"
#include "../Utilities/Color.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Debug.h"
#include "../Utilities/Convert.h"

CTileMap::CTileMap(string mapFile, string tilesetFile)
{
	this->mapFile = mapFile;
	this->tilesetFile = tilesetFile;
	this->mapRows = 0;
	this->mapColumns = 0;
	this->tilesetRows = 0;
	this->tilesetColumns = 0;

	LoadMap();
	LoadTileset();
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
	TiXmlElement* column = nullptr;

	root->QueryIntAttribute("tileWidth", &tileWidth);
	root->QueryIntAttribute("tileHeight", &tileHeight);
	root->QueryIntAttribute("tilesetWidth", &tilesetWidth);
	root->QueryIntAttribute("tilesetHeight", &tilesetHeight);

	int rowsCount = 0;
	int columnsCount = 0;

	for (row = root->FirstChildElement(); row != nullptr; row = row->NextSiblingElement())
	{
		vector<int> columns;

		for (column = row->FirstChildElement(); column != nullptr; column = column->NextSiblingElement())
		{
			int value = stoi(column->GetText());

			columns.emplace_back(value);
		}

		if (columns.size() > columnsCount)
		{
			columnsCount = columns.size();
		}

		map.emplace_back(columns);

		rowsCount++;
	}

	width = tileWidth * columnsCount;
	height = tileHeight * rowsCount;
	mapRows = height / tileHeight;
	mapColumns = width / tileWidth;
	tilesetRows = tilesetHeight / tileHeight;
	tilesetColumns = tilesetWidth / tileWidth;
}

void CTileMap::LoadTileset()
{
	CTextureManager* textureManager = CTextureManager::GetInstance();

	textureManager->Add("tileset_texture", CConvert::s2ws(tilesetFile).c_str(), CColor::FromRgb(255, 0, 255));
	LPDIRECT3DTEXTURE9 tileset = textureManager->Get("tileset_texture");

	int count = 0;

	for (int row = 0; row < tilesetRows; row++)
	{
		for (int column = 0; column < tilesetColumns; column++)
		{
			string tileId = "tile_" + to_string(count);

			int left = column * tileWidth;
			int top = row * tileHeight;
			int right = left + tileWidth;
			int bottom = top + tileHeight;

			CSpriteManager::GetInstance()->Add(tileId, left, top, right, bottom, tileset);

			count++;
		}
	}
}

void CTileMap::Render(CCamera* camera)
{
	int startingColumn = camera->GetLeft() / tileWidth;
	int endingColumn = camera->GetRight() / tileWidth;

	if (endingColumn >= mapColumns)
	{
		endingColumn = mapColumns - 1;
	}

	for (int row = 0; row < mapRows; row++)
	{
		for (int column = startingColumn; column <= endingColumn; column++)
		{
			string tileId = "tile_" + to_string(map[row][column]);

			float x = tileWidth * column;
			float y = tileHeight * row;

			CSpriteManager::GetInstance()->Get(tileId)->Draw(x, y + BLACKBOARD_HEIGHT);
		}
	}
}

int CTileMap::GetWidth()
{
	return width;
}

int CTileMap::GetHeight()
{
	return height;
}

CTileMap::~CTileMap()
{
	for (int i = 0; i < map.size(); i++)
	{
		map[i].clear();
	}

	map.clear();
}
