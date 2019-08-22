#pragma once

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../Game.h"
#include "./TransformComponent.h"

class ColliderComponent : public Component {
 public:
  std::string ColliderTag;
  SDL_Rect Collider;
  SDL_Rect SourceRectangle;
  SDL_Rect DestinationRectangle;
  TransformComponent* Transform;

  ColliderComponent(std::string colliderTag, int x, int y, int width,
                    int height) {
    this->ColliderTag = colliderTag;
    this->Collider = {x, y, width, height};
  }

  void Initialize() override {
    if (owner->HasComponent<TransformComponent>()) {
      Transform = owner->GetComponent<TransformComponent>();
      SourceRectangle = {0, 0, Transform->Width, Transform->Height};
      DestinationRectangle = {Collider.x, Collider.y, Collider.w, Collider.h};
    }
  }

  void Update(float deltaTime) override {
    Collider.x = static_cast<int>(Transform->Position.x);
    Collider.y = static_cast<int>(Transform->Position.y);
    Collider.w = Transform->Width * Transform->Scale;
    Collider.h = Transform->Height * Transform->Scale;
    DestinationRectangle.x = Collider.x - Game::Camera.x;
    DestinationRectangle.y = Collider.y - Game::Camera.y;
  }
};