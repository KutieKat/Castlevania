#pragma once
#include <vector>
#include <string>

#include "../Sprites/SpriteManager.h"
#include "../Camera/Camera.h"
using namespace std;

class CTileMap
{
public:
	CTileMap(string mapFile, LPWSTR tilesFile);

	void Render(CCamera* camera);

	int GetWidth();
	int GetHeight();

protected:
	vector<vector<int>> map;

	string mapFile;
	int mapColumns;
	int mapRows;

	LPWSTR tilesetFile;
	int tileWidth;
	int tileHeight;

	int tilesetRows;
	int tilesetColumns;
	int tilesetWidth;
	int tilesetHeight;

	int width;
	int height;

	void LoadMap();
	void LoadTiles();
};

