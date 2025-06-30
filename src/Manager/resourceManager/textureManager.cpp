#include "Manager/resourceManager/textureManager.h"
#include <iostream>
void TextureManager::LoadTexture(const std::string& filename, const TextureLevel level)
{
    if (textures.count(filename) == 0) {
        std::cout << filename;
        if (!textures[filename].loadFromFile(filename)) {
            throw std::runtime_error("Failed to load texture: " + filename);
        };
        texture_levels[filename] = level;
    }
}

void TextureManager::UnloadSpecificTextures()
{
    for (auto it = texture_levels.begin(); it != texture_levels.end();) {
        if (it->second == TextureLevel::Specific) {
            textures.erase(it->first);
            it = texture_levels.erase(it);
        }
        else {
            ++it;
        }
    }
}