#include "./Game.h"
#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./Components/TransformComponent.h"
#include "./Constants.h"
#include "EntityManager.h"

EntityManager manager;
SDL_Renderer* Game::Renderer;

Game::Game() { this->m_isRunning = false; }

Game::~Game() {}

bool Game::IsRunning() const { return this->m_isRunning; }

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 20.0f;
float projectileVelY = 30.0f;

void Game::Initialize(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }

  m_window = SDL_CreateWindow("MyTitle", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_BORDERLESS);

  if (!m_window) {
    std::cerr << "Error creating SDL window." << std::endl;
    return;
  }

  Renderer = SDL_CreateRenderer(m_window, -1, 0);
  if (!Renderer) {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  LoadLevel(0);

  m_isRunning = true;
  return;
}

void Game::LoadLevel(int levelNumber) {
  Entity& newEntity(manager.AddEntity("projectile"));
  Entity& newEntity2(manager.AddEntity("projectile2"));
  newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
  newEntity2.AddComponent<TransformComponent>(200, 0, -20, 20, 16, 16, 1);

  std::vector<Entity*> entities = manager.GetEntities();
  for (size_t i = 0; i < entities.size(); i++) {
    std::cout << "Entity name " << i + 1 << ": " << entities[i]->Name << std::endl;
    for(Component* component : entities[i]->GetComponents()){
      std::cout << "Component name: " << component->componentName << std::endl;
    }
    std::cout << std::endl;
  }
}

void Game::ProcessInput() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT: {
      m_isRunning = false;
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
    }
    default:
      break;
  }
}

void Game::Update() {
  // Wait until 16ms has ellapsed since the last frame.
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksLastFrame + FRAME_TARGET_TIME))
    ;

  // Delta time is the difference in ticks from the last frame converted to
  // seconds.
  float deltaTime = (SDL_GetTicks() - TicksLastFrame) / 1000.0f;

  // Clamp deltaTime to a maximum value.
  deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

  // Sets the new ticks for the current frame to be used in the next pass.
  TicksLastFrame = SDL_GetTicks();

  manager.Update(deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(Renderer, 21, 21, 21, 255);
  SDL_RenderClear(Renderer);

  if (manager.HasNoEntities()) {
    return;
  }

  manager.Render();

  SDL_RenderPresent(Renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}