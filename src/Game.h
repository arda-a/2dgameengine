#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
 private:
  bool m_isRunning;
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  int m_ticksLastFrame;

 public:
  Game();
  ~Game();
  bool IsRunning() const;
  void Initialize(int width, int height);
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();
};