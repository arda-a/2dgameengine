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
  for (int i = 0; i < m_entities.size(); i++) {
    m_entities[i]->Update(deltaTime);
    
    //Destroy inactive entity
    if(!m_entities[i]->IsActive()){
      m_entities.erase(m_entities.begin() + i);
    }
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

// Currently O(n2) complexity. TODO : Reduce this.
CollisionType EntityManager::CheckCollisions() const {
  for (auto& thisEntity : m_entities) {
    if (thisEntity->HasComponent<ColliderComponent>()) {
      ColliderComponent* thisCollider =
          thisEntity->GetComponent<ColliderComponent>();
      for (auto& otherEntity : m_entities) {
        if (thisEntity->Name.compare(otherEntity->Name) != 0 &&
            otherEntity->HasComponent<ColliderComponent>()) {
          ColliderComponent* otherCollider =
              otherEntity->GetComponent<ColliderComponent>();
          if (Collision::CheckRectangleCollision(thisCollider->Collider,
                                                 otherCollider->Collider)) {
            if (thisCollider->ColliderTag.compare("player") == 0 &&
                otherCollider->ColliderTag.compare("enemy") == 0) {
              return PLAYER_ENEMY_COLLISION;
            }
            if (thisCollider->ColliderTag.compare("player") == 0 &&
                otherCollider->ColliderTag.compare("PROJECTILE") == 0) {
              return PLAYER_PROJECTILE_COLLISION;
            }
            if (thisCollider->ColliderTag.compare("enemy") == 0 &&
                otherCollider->ColliderTag.compare("FRIENDLY_PROJECTILE") ==
                    0) {
              return ENEMY_PROJECTILE_COLLISION;
            }
            if (thisCollider->ColliderTag.compare("player") == 0 &&
                otherCollider->ColliderTag.compare("LEVEL_COMPLETE") == 0) {
              return PLAYER_LEVEL_COMPLETE_COLLISION;
            }
          }
        }
      }
    }

    return NO_COLLISION;
  }
}