#pragma once

#include "../lib/lua/sol.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class AssetManager;

class Game {
private:
  bool m_isRunning;
  SDL_Window *m_window;

public:
  Game();
  ~Game();
  bool IsRunning() const;
  void Initialize(int width, int height);
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();
  void LoadLevel(int levelNumber);
  void HandleCameraMovement();
  void CheckCollisions();
  void ProcessGameOver();
  void ProcessNextLevel(int levelNumber);
  int TicksLastFrame = 0;
  static SDL_Renderer *Renderer;
  static AssetManager *assetManager;
  static SDL_Event Event;
  static SDL_Rect Camera;
};