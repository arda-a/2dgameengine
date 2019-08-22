#pragma once

#include <map>
#include <string>
#include <vector>
#include "./EntityManager.h"
#include "./Constants.h"
#include "Component.h"

class Component;
class EntityManager;

class Entity {
 private:
  EntityManager& m_manager;
  bool m_isActive;
  std::vector<Component*> m_components;
  std::map<const std::type_info*, Component*> m_componentTypeMap;

 public:
  std::string Name;
  LayerType Layer;
  Entity(EntityManager& manager);
  Entity(EntityManager& manager, std::string name, LayerType layer);
  void Update(float deltaTime);
  void Render();
  void Destroy();
  bool IsActive() const;
  std::vector<Component*> GetComponents() const { return m_components; }
  void ListAllComponents() const;
  
  template <typename T>
  bool HasComponent() {
    return m_componentTypeMap[&typeid(T)];
  }

  template <typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args) {
    T* newComponent(new T(std::forward<TArgs>(args)...));
    newComponent->componentName = typeid(T).name();
    newComponent->owner = this;
    m_components.emplace_back(newComponent);
    m_componentTypeMap[&typeid(*newComponent)] = newComponent;
    newComponent->Initialize();
    return *newComponent;
  }

  template <typename T>
  T* GetComponent() {
    return static_cast<T*>(m_componentTypeMap[&typeid(T)]);
  }
};