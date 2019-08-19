#include "./EntityManager.h"
#include "./Constants.h"
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
  for(int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++){
    for (auto& entity : GetEntitiesByLayer(static_cast<LayerType>(layerNumber))){
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
  for(auto& entity: m_entities){
    if(entity->Layer == layer){
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