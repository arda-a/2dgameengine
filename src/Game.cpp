#include "./Game.h"
#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./Constants.h"

Game::Game() { this->m_isRunning = false; }

Game::~Game() {}

bool Game::IsRunning() const { return this->m_isRunning; }

glm::vec2 projectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVel = glm::vec2(20.0f, 20.0f);

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

  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  if (!m_renderer) {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  m_isRunning = true;
  return;
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
  while (
      !SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksLastFrame + FRAME_TARGET_TIME))
    ;

  // Delta time is the difference in ticks from the last frame converted to
  // seconds.
  float deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.0f;

  // Clamp deltaTime to a maximum value.
  deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

  // Sets the new ticks for the current frame to be used in the next pass.
  m_ticksLastFrame = SDL_GetTicks();

  projectilePos = glm::vec2(projectilePos.x + projectileVel.x * deltaTime,
                            projectilePos.y + projectileVelY * deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
  SDL_RenderClear(m_renderer);

  SDL_Rect projectile{(int)projectilePos.x, (int)projectilePos.y, 10, 10};

  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(m_renderer, &projectile);

  SDL_RenderPresent(m_renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}