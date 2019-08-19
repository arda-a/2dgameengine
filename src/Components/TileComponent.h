#pragma once

#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"
#include "../AssetManager.h"
#include "../EntityManager.h"

class TileComponent : public Component {
 public:
  SDL_Texture *Texture;
  SDL_Rect SourceRectangle;
  SDL_Rect DestinationRectangle;
  glm::vec2 Position;

  TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize,
                int tileScale, std::string assetTextureId) {
    Texture = Game::assetManager->GetTexture(assetTextureId);

    SourceRectangle.x = sourceRectX;
    SourceRectangle.y = sourceRectY;
    SourceRectangle.w = tileSize;
    SourceRectangle.h = tileSize;

    DestinationRectangle.x = x;
    DestinationRectangle.y = y;
    DestinationRectangle.w = tileSize * tileScale;
    DestinationRectangle.h = tileSize * tileScale;

    Position.x = x;
    Position.y = y;
  }

  ~TileComponent() { SDL_DestroyTexture(Texture); }
  void Update(float deltaTime) override {
      //TODO : Here is where we will take care of the tile positions
      // based on the camera control that will keep moving around.
  }

  void Render() override {
    TextureManager::Draw(Texture, SourceRectangle, DestinationRectangle,
                         SDL_FLIP_NONE);
  }
};