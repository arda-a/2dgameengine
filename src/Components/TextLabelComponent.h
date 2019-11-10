#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../AssetManager.h"
#include "../EntityManager.h"
#include "../FontManager.h"
#include "../Game.h"

class TextLabelComponent : public Component {
 private:
  SDL_Rect m_position;
  std::string m_text;
  std::string m_fontFamily;
  SDL_Color m_color;
  SDL_Texture* m_texture;

 public:
  TextLabelComponent(int x, int y, std::string text, std::string fontFamily,
                     const SDL_Color& color) {
    this->m_position.x = x;
    this->m_position.y = y;
    this->m_text = text;
    this->m_fontFamily = fontFamily;
    this->m_color = color;
    SetLabelText(text, fontFamily);
  }

  void SetLabelText(std::string text, std::string fontFamily) {
    SDL_Surface* surface = TTF_RenderText_Blended(
        Game::assetManager->GetFont(fontFamily), text.c_str(), m_color);
    m_texture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(m_texture, NULL, NULL, &m_position.w, &m_position.h);
  }

  void Render() override { FontManager::Draw(m_texture, m_position); }
};
