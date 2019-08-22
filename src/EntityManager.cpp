#include "./EntityManager.h"
#include <iostream>
#include "./Collision.h"
#include "./Components/ColliderComponent.h"
#include "./Constants.h"

void EntityManager::ClearData() {
  for (auto& entity : m_entities) {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities() { return m_entities.size() == 0; }

void EntityManager::Update(float deltaTime) {
  for (auto& entity : m_entities) {
    entity->Update(deltaTime);
  }
}

void EntityManager::Render() {
  for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
    for (auto& entity :
         GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
      entity->Render();
    }
  }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
  Entity* entity = new Entity(*this, entityName, layer);
  m_entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const { return m_entities; }

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
  std::vector<Entity*> selectedEntities;
  for (auto& entity : m_entities) {
    if (entity->Layer == layer) {
      selectedEntities.emplace_back(entity);
    }
  }
  return selectedEntities;
}

void EntityManager::ListAllEntities() const {
  unsigned int i = 0;
  for (auto& entity : m_entities) {
    std::cout << "Entity[" << i << "]" << entity->Name << std::endl;
  }
}

unsigned int EntityManager::GetEntityCount() { return m_entities.size(); }

std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
  ColliderComponent* collider = myEntity.GetComponent<ColliderComponent>();
  for (auto& entity : m_entities) {
    if (entity->Name.compare(myEntity.Name) != 0 &&
        entity->Name.compare("Tile") != 0) {
            if (entity->HasComponent<ColliderComponent>()) {
        ColliderComponent* otherCollider =
            entity->GetComponent<ColliderComponent>();
        if (Collision::CheckRectangleCollision(collider->Collider,
                                               otherCollider->Collider)) {
          return otherCollider->ColliderTag;
        }
      }
    }
  }
  return std::string();
}