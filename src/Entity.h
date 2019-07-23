#pragma once

#include <string>
#include <vector>
#include "./EntityManager.h"
#include "Component.h"

class Component;
class EntityManager;

class Entity {
 private:
  EntityManager& m_manager;
  bool m_isActive;
  std::vector<Component*> m_components;

 public:
  std::string Name;
  Entity(EntityManager& manager);
  Entity(EntityManager& manager, std::string name);
  void Update(float deltaTime);
  void Render();
  void Destroy();
  bool IsActive() const;

  template <typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args){
    T* newComponent(new T(std::forward<TArgs>(args)...));
    newComponent->owner = this;
    m_components.emplace_back(newComponent);
    newComponent->Initialize();
    return *newComponent;
  }
};