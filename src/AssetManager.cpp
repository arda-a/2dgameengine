#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager* manager) : m_manager(manager) {}

void AssetManager::ClearData() {
  m_textures.clear();
  m_fonts.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* filePath) {
  m_textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

void AssetManager::AddFont(std::string fontId, const char* filePath,
                           int fontSize) {
  m_fonts.emplace(fontId, FontManager::LoadFont(filePath, fontSize));
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
  return m_textures[textureId];
}

TTF_Font* AssetManager::GetFont(std::string fontId) { return m_fonts[fontId]; }