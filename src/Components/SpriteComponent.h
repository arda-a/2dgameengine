#pragma once

#include <SDL2/SDL.h>
#include <map>
#include "../Animation.h"
#include "../Component.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"

class SpriteComponent : public Component {
 private:
  TransformComponent* m_transform;
  SDL_Texture* m_texture;
  SDL_Rect m_sourceRectangle;
  SDL_Rect m_destinationRectangle;
  bool m_isAnimated;
  int m_numFrames;
  int m_animationSpeed;
  bool m_isFixed;
  std::map<std::string, Animation> m_animations;
  std::string m_currentAnimationName;
  unsigned int m_animationIndex;

 public:
  SDL_RendererFlip SpriteFlip = SDL_FLIP_NONE;

  SpriteComponent(std::string assetTextureId) {
    m_isAnimated = false;
    m_isFixed = false;
    SetTexture(assetTextureId);
  }

  SpriteComponent(std::string id, int numFrames, int animationSpeed,
                  bool hasDirections, bool isFixed) {
    m_isAnimated = true;
    this->m_numFrames = numFrames;
    this->m_animationSpeed = animationSpeed;
    this->m_isFixed = isFixed;

    if (hasDirections) {
      Animation downAnimation = Animation(0, numFrames, animationSpeed);
      Animation rightAnimation = Animation(1, numFrames, animationSpeed);
      Animation leftAnimation = Animation(2, numFrames, animationSpeed);
      Animation upAnimation = Animation(3, numFrames, animationSpeed);

      m_animations.emplace("DownAnimation", downAnimation);
      m_animations.emplace("RightAnimation", rightAnimation);
      m_animations.emplace("LeftAnimation", leftAnimation);
      m_animations.emplace("UpAnimation", upAnimation);

      this->m_animationIndex = 0;
      this->m_currentAnimationName = "DownAnimation";
    } else {
      Animation singleAnimation = Animation(0, numFrames, animationSpeed);
      m_animations.emplace("SingleAnimation", singleAnimation);
      this->m_animationIndex = 0;
      this->m_currentAnimationName = "SingleAnimation";
    }

    Play(this->m_currentAnimationName);

    SetTexture(id);
  }

  void Play(std::string animationName) {
    m_numFrames = m_animations[animationName].NumFrames;
    m_animationIndex = m_animations[animationName].Index;
    m_animationSpeed = m_animations[animationName].AnimationSpeed;
    m_currentAnimationName = animationName;
  }

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
    if (m_isAnimated) {
      m_sourceRectangle.x = m_sourceRectangle.w *
                            static_cast<int>((SDL_GetTicks() / m_animationSpeed) % m_numFrames);
    }
    m_sourceRectangle.y = m_animationIndex * m_transform->Height;

    m_destinationRectangle.x = static_cast<int>(m_transform->Position.x);
    m_destinationRectangle.y = static_cast<int>(m_transform->Position.y);
    m_destinationRectangle.w = m_transform->Width;
    m_destinationRectangle.h = m_transform->Height;
  }

  void Render() override {
    TextureManager::Draw(m_texture, m_sourceRectangle, m_destinationRectangle,
                         SpriteFlip);
  }
};