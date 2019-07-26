#pragma once

#include <SDL2/SDL.h>
#include "../Component.h"
#include "../Game.h"
#include "./TransformComponent.h"
#include "../TextureManager.h"

class SpriteComponent : public Component {
 private:
  TransformComponent* m_transform;
  SDL_Texture* m_texture;
  SDL_Rect m_sourceRectangle;
  SDL_Rect m_destinationRectangle;

 public:
  SDL_RendererFlip SpriteFlip = SDL_FLIP_NONE;

  SpriteComponent(const char* filePath) { SetTexture(filePath); }

  void SetTexture(std::string assetTextureId) {
    m_texture = Game::assetManager->GetTexture(assetTextureId);
  }

  void Initialize() override {
    m_transform = owner->GetComponent<TransformComponent>();
    m_sourceRectangle.x = 0;
    m_sourceRectangle.y = 0;
    m_sourceRectangle.w = m_transform->Width;
    m_sourceRectangle.h = m_transform->Height;
  }

  void Update(float deltaTime) override {
    m_destinationRectangle.x = (int)m_transform->Position.x;
    m_destinationRectangle.y = (int)m_transform->Position.y;
    m_destinationRectangle.w = m_transform->Width;
    m_destinationRectangle.h = m_transform->Height;
  }

  void Render() override {
      TextureManager::Draw(m_texture, m_sourceRectangle, m_destinationRectangle, SpriteFlip);
  }
};