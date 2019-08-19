#include "./Map.h"
#include "./EntityManager.h"
#include "./Game.h"
#include "Components/TileComponent.h"
#include <fstream>

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize) {
  this->m_textureId = textureId;
  this->m_scale = scale;
  this->m_tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
  std::fstream mapFile;
  mapFile.open(filePath);

  for (size_t y = 0; y < mapSizeY; y++) {
    for (size_t x = 0; x < mapSizeX; x++) {
      char ch;
      mapFile.get(ch);
      int sourceRectY = atoi(&ch) * m_tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * m_tileSize;
      AddTile(sourceRectX, sourceRectY, x * m_scale * m_tileSize,
              y * m_scale * m_tileSize);
      mapFile.ignore();
    }
  }
  mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
  Entity& newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
  newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y,
                                      m_tileSize, m_scale, m_textureId);
}