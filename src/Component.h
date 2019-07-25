#pragma once

#include <string>

class Entity;

class Component {
 public:
  Entity* owner;
  std::string componentName;
  virtual ~Component(){}
  virtual void Initialize(){}
  virtual void Update(float deltaTime){}
  virtual void Render(){}
};