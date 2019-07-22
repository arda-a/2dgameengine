#pragma once

#include <vector>
#include "./Component.h"
#include "./Entity.h"

class EntityManager {
 private:
  std::vector<Entity*> m_entities;

 public:
  void ClearData();
  void Update(float deltaTime);
  void Render();
  bool HasNoEntities();
  Entity& AddEntity(std::string entityName);
  std::vector<Entity*> GetEntities() const;
  unsigned int GetEntityCount();
};