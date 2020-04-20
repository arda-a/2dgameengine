#include "./Game.h"
#include "../lib/glm/glm.hpp"
#include "./AssetManager.h"
#include "./Components/ColliderComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/TransformComponent.h"
#include "./Constants.h"
#include "EntityManager.h"
#include "Map.h"
#include <iostream>

EntityManager manager;
AssetManager *Game::assetManager = new AssetManager(&manager);
SDL_Renderer *Game::Renderer;
SDL_Event Game::Event;
SDL_Rect Game::Camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity *mainPlayer = NULL;
Map *map;

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

  if (TTF_Init() != 0) {
    std::cerr << "Error initializing SDL TTF" << std::endl;
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

  LoadLevel(1);

  m_isRunning = true;
  return;
}

void Game::LoadLevel(int levelNumber) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

  std::string levelName = "Level" + std::to_string(levelNumber);
  lua.script_file("./assets/scripts/" + levelName + ".lua");

  /******************************************/
  /* LOADING ASSETS FROM LUA CONFIG FILE    */
  /******************************************/

  sol::table levelData = lua[levelName];
  sol::table levelAssets = levelData["assets"];

  unsigned int assetIndex = 0;
  while (true) {
    sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
    if (existsAssetIndexNode == sol::nullopt)
      break;
    else {
      sol::table asset = levelAssets[assetIndex];
      std::string assetType = asset["type"];
      if (assetType.compare("texture") == 0) {
        std::string assetId = asset["id"];
        std::string assetFile = asset["file"];
        assetManager->AddTexture(assetId, assetFile.c_str());
      }
      if (assetType.compare("font") == 0) {
        std::string assetId = asset["id"];
        std::string assetFile = asset["file"];
        int fontSize = asset["fontSize"];
        assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
      }
    }
    assetIndex++;
  }

  /******************************************/
  /* LOADING MAPS AND TILES FROM LUA CONFIG FILE    */
  /******************************************/

  sol::table levelMap = levelData["map"];
  std::string mapTextureId = levelMap["textureAssetId"];
  std::string mapFile = levelMap["file"];

  map = new Map(mapTextureId,
                static_cast<int>(levelMap["scale"]),
                static_cast<int>(levelMap["tileSize"]));
  map->LoadMap(mapFile,
               static_cast<int>(levelMap["mapSizeX"]),
               static_cast<int>(levelMap["mapSizeY"]));

  /******************************************/
  /* LOAD ENTITIES AND COMPONENTS FROM LUA CONFIG FILE  */
  /******************************************/
  sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt) {
            break;
        }
        else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // Add new entity
            auto& newEntity(manager.AddEntity(entityName, entityLayerType));

            // Add Transform Component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            // Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId);
                }
            }

            // Add input control component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                    std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                    std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                    std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                    std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                    newEntity.AddComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey);
                }
            }

            // Add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                newEntity.AddComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"])
                );
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.AddComponent<SpriteComponent>(textureAssetId);
                projectile.AddComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }

    mainPlayer = manager.GetEntityByName("player");
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
  //Sleep the execution until we reach the target frame time in milliseconds
  int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - TicksLastFrame);

  //Only call delay if we are too fast to process this frame
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
    SDL_Delay(timeToWait);
  }

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
  if (mainPlayer) {
    TransformComponent *mainPlayerTransform =
        mainPlayer->GetComponent<TransformComponent>();

    Camera.x = mainPlayerTransform->Position.x - (WINDOW_WIDTH / 2);
    Camera.y = mainPlayerTransform->Position.y - (WINDOW_HEIGHT / 2);

    // Clamp the values horizontally and vertically
    Camera.x = Camera.x < 0 ? 0 : Camera.x;
    Camera.y = Camera.y < 0 ? 0 : Camera.y;
    Camera.x = Camera.x > Camera.w ? Camera.w : Camera.x;
    Camera.y = Camera.y > Camera.h ? Camera.h : Camera.y;
  }
}

void Game::CheckCollisions() {
  CollisionType collisionType = manager.CheckCollisions();
  if (collisionType == PLAYER_ENEMY_COLLISION) {
    ProcessGameOver();
  }
  if (collisionType == PLAYER_PROJECTILE_COLLISION) {
    ProcessGameOver();
  }
  if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
    ProcessNextLevel(1);
  }
}

void Game::ProcessNextLevel(int levelNumber) {
  std::cout << "Next Level" << std::endl;
  m_isRunning = false;
}

void Game::ProcessGameOver() {
  std::cout << "Game Over" << std::endl;
  m_isRunning = false;
}

void Game::Destroy() {
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}