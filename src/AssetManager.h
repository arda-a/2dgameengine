#pragma once

#include <map>
#include <string>
#include "./EntityManager.h"
#include "./TextureManager.h"

class AssetManager {
 private:
  EntityManager* m_manager;
  std::map<std::string, SDL_Texture*> m_textures;

 public:
    AssetManager(EntityManager* manager);
    ~AssetManager();
    void ClearData();
    void AddTexture(std::string textureId, const char* filePath);
    SDL_Texture* GetTexture(std::string textureId);
};