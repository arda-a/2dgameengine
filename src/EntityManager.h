#pragma once

#include <vector>
#include "./Component.h"
#include "./Constants.h"
#include "./Entity.h"

class EntityManager {
 private:
  std::vector<Entity*> m_entities;

 public:
  void ClearData();
  void Update(float deltaTime);
  void Render();
  bool HasNoEntities();
  Entity& AddEntity(std::string entityName, LayerType layer);
  std::vector<Entity*> GetEntities() const;
  std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
  void ListAllEntities() const;
  unsigned int GetEntityCount();
  CollisionType CheckCollisions() const;
};