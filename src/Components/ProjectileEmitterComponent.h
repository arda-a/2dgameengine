#pragma once

#include "../../lib/glm/glm.hpp"
#include "../Components/TransformComponent.h"
#include "../EntityManager.h"

class ProjectileEmitterComponent : public Component {
private:
  TransformComponent *m_transform;
  glm::vec2 m_origin;
  int m_speed;
  int m_range;
  float m_angleRad;
  bool m_shouldLoop;

public:
  ProjectileEmitterComponent(int speed, int angleDeg, int range, bool shouldLoop)
      : m_speed(speed), m_range(range), m_shouldLoop(shouldLoop) {
    this->m_angleRad = glm::radians(static_cast<float>(angleDeg));
  }

  void Initialize() override {
    m_transform = owner->GetComponent<TransformComponent>();
    m_origin = glm::vec2(m_transform->Position.x, m_transform->Position.y);
    m_transform->Velocity = glm::vec2(glm::cos(m_angleRad) * m_speed, glm::sin(m_angleRad) * m_speed);
  }

  void Update(float deltaTime) override {
      if(glm::distance(m_transform->Position, m_origin) > m_range){
          if(m_shouldLoop){
              m_transform->Position.x = m_origin.x;
              m_transform->Position.y = m_origin.y;
          }
          else{
              owner->Destroy();
          }
      }
  }
};