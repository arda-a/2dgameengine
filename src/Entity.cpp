#include "Entity.h"
#include <iostream>

Entity::Entity(EntityManager& manager) : m_manager(manager) {
  this->m_isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name)
    : m_manager(manager), Name(name) {
  this->m_isActive = true;
}

void Entity::Update(float deltaTime) {
  for (auto& component : m_components) {
    component->Update(deltaTime);
  }
}

void Entity::Render() {
  for (auto& component : m_components) {
    component->Render();
  }
}

void Entity::Destroy() { this->m_isActive = false; }

bool Entity::IsActive() const { return this->m_isActive; }

void Entity::ListAllComponents() const {
  for (auto mapElement : m_componentTypeMap) {
    std::cout << "  Component<" << mapElement.first->name() << ">" << std::endl;
  }
}