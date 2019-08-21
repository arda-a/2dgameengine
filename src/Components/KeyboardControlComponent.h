#pragma once

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../EntityManager.h"
#include "../Game.h"

class KeyboardControlComponent : public Component {
 public:
  std::string UpKey;
  std::string DownKey;
  std::string RightKey;
  std::string LeftKey;
  std::string ShootKey;
  TransformComponent *Transform;
  SpriteComponent *Sprite;

  KeyboardControlComponent() {}

  KeyboardControlComponent(std::string upkey, std::string rightkey,
                           std::string downkey, std::string leftkey,
                           std::string shootKey) {
    this->UpKey = GetSDLKeyStringCode(upkey);
    this->RightKey = GetSDLKeyStringCode(rightkey);
    this->DownKey = GetSDLKeyStringCode(downkey);
    this->LeftKey = GetSDLKeyStringCode(leftkey);
    this->ShootKey = GetSDLKeyStringCode(shootKey);
  }

  std::string GetSDLKeyStringCode(std::string key) {
    if (key.compare("down") == 0) return "1073741905";
    if (key.compare("up") == 0) return "1073741906";
    if (key.compare("left") == 0) return "1073741904";
    if (key.compare("right") == 0) return "1073741903";
    if (key.compare("space") == 0) return "32";
    return std::to_string(static_cast<int>(key[0]));
  }

  void Initialize() override {
    Transform = owner->GetComponent<TransformComponent>();
    Sprite = owner->GetComponent<SpriteComponent>();
  }

  void Update(float deltaTime) override {
    if (Game::Event.type == SDL_KEYDOWN) {
      std::string keyCode =  std::to_string(Game::Event.key.keysym.sym);

      if (keyCode.compare(UpKey) == 0) {
        Transform->Velocity.y = -40;
        Transform->Velocity.x = 0;
        Sprite->Play("UpAnimation");
      }
      if (keyCode.compare(RightKey) == 0) {
        Transform->Velocity.y = 0;
        Transform->Velocity.x = 40;
        Sprite->Play("RightAnimation");
      }
      if (keyCode.compare(DownKey) == 0) {
        Transform->Velocity.y = 40;
        Transform->Velocity.x = 0;
        Sprite->Play("DownAnimation");
      }
      if (keyCode.compare(LeftKey) == 0) {
        Transform->Velocity.y = 0;
        Transform->Velocity.x = -40;
        Sprite->Play("LeftAnimation");
      }
      if (keyCode.compare(ShootKey) == 0) {
        // TODO

        // Shoot projectiles when 'shoot' key is pressed.
      }
    }

    if (Game::Event.type == SDL_KEYUP) {
      std::string keyCode = std::to_string(Game::Event.key.keysym.sym);
      if (keyCode.compare(UpKey) == 0) {
        Transform->Velocity.y = 0;
      }
      if (keyCode.compare(RightKey) == 0) {
        Transform->Velocity.x = 0;
      }
      if (keyCode.compare(DownKey) == 0) {
        Transform->Velocity.y = 0;
      }
      if (keyCode.compare(LeftKey) == 0) {
        Transform->Velocity.x = 0;
      }
      if (keyCode.compare(ShootKey) == 0) {
        // TODO
      }
    }
  }
};