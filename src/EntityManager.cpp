#include "./EntityManager.h"
#include <iostream>

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
  for (auto& entity : m_entities) {
    entity->Render();
  }
}

Entity& EntityManager::AddEntity(std::string entityName) {
  Entity* entity = new Entity(*this, entityName);
  m_entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const { return m_entities; }

void EntityManager::ListAllEntities() const {
  unsigned int i = 0;
  for (auto& entity : m_entities) {
    std::cout << "Entity[" << i << "]" << entity->Name << std::endl;
  }
}

unsigned int EntityManager::GetEntityCount() { return m_entities.size(); }