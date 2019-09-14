#include "./Game.h"
#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./AssetManager.h"
#include "./Components/ColliderComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TransformComponent.h"
#include "./Constants.h"
#include "EntityManager.h"
#include "Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::Renderer;
SDL_Event Game::Event;
SDL_Rect Game::Camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

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

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
  /* Start including new assets to the assetmanager list */
  assetManager->AddTexture(
      "tank-image", std::string("./assets/images/tank-big-right.png").c_str());
  assetManager->AddTexture(
      "chopper-image",
      std::string("./assets/images/chopper-spritesheet.png").c_str());
  assetManager->AddTexture("radar-image",
                           std::string("./assets/images/radar.png").c_str());
  assetManager->AddTexture("jungle-tiletexture",
                           std::string("./assets/tilemaps/jungle.png").c_str());

  map = new Map("jungle-tiletexture", 2, 32);
  map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

  player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
  player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
  player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left",
                                                "space");
  player.AddComponent<ColliderComponent>("player", 240, 106, 32, 32);

  Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
  tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
  tankEntity.AddComponent<SpriteComponent>("tank-image");
  tankEntity.AddComponent<ColliderComponent>("enemy", 150, 495, 32, 32);

  Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
  radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
  radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

  std::vector<Entity*> entities = manager.GetEntities();
  for (size_t i = 0; i < entities.size(); i++) {
    std::cout << "Entity name " << i + 1 << ": " << entities[i]->Name
              << std::endl;
    for (Component* component : entities[i]->GetComponents()) {
      std::cout << "Component name: " << component->componentName << std::endl;
    }
    std::cout << std::endl;
  }
}

void Game::ProcessInput() {
  SDL_PollEvent(&Event);
  switch (Event.type) {
    case SDL_QUIT: {
      m_isRunning = false;
      break;
    }
    case SDL_KEYDOWN: {
      if (Event.key.keysym.sym == SDLK_ESCAPE) {
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

  HandleCameraMovement();
  CheckCollisions();
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

void Game::HandleCameraMovement() {
  TransformComponent* mainPlayerTransform =
      player.GetComponent<TransformComponent>();

  Camera.x = mainPlayerTransform->Position.x - (WINDOW_WIDTH / 2);
  Camera.y = mainPlayerTransform->Position.y - (WINDOW_HEIGHT / 2);

  // Clamp the values horizontally and vertically
  Camera.x = Camera.x < 0 ? 0 : Camera.x;
  Camera.y = Camera.y < 0 ? 0 : Camera.y;
  Camera.x = Camera.x > Camera.w ? Camera.w : Camera.x;
  Camera.y = Camera.y > Camera.h ? Camera.h : Camera.y;
}

void Game::CheckCollisions() {
  CollisionType collisionType = manager.CheckCollisions();
  if(collisionType == PLAYER_ENEMY_COLLISION){
    ProcessGameOver();
  }
  if(collisionType == PLAYER_LEVEL_COMPLETE_COLLISION){
    ProcessNextLevel(1);
  }
}

void Game::ProcessNextLevel(int levelNumber){
  std::cout << "Next Level" << std::endl;
  m_isRunning = false;
}

void Game::ProcessGameOver(){
  std::cout << "Game Over" << std::endl;
  m_isRunning = false;
}

void Game::Destroy() {
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}