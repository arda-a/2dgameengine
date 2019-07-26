#pragma once

#include "../Game.h"
#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"

class TransformComponent : public Component {
 public:
  glm::vec2 Position;
  glm::vec2 Velocity;
  int Width;
  int Height;
  int Scale;

  TransformComponent(int posX, int poxY, int velX, int velY, int w, int h,
                     int s) {
    Position = glm::vec2(posX, poxY);
    Velocity = glm::vec2(velX, velY);
    Width = w;
    Height = h;
    Scale = s;
  }

  void Initialize() override {}

  void Update(float deltaTime) override {
    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
  }

  void Render() override {
  }
};
