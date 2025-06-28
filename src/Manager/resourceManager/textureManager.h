#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

enum class TextureLevel
{
    Common,
    Specific
};
class TextureManager
{
public:
    sf::Texture& getTexture(const std::string& filename) { return textures[filename]; }
    void LoadTexture(const std::string& filename, const TextureLevel level = TextureLevel::Common)
    {
        if (textures.count(filename) == 0) {
            if (!textures[filename].loadFromFile(filename)) {
                throw std::runtime_error("Failed to load texture: " + filename);
            };
            texture_levels[filename] = level;
        }
    }

    void UnloadSpecificTextures()
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

private:
    std::unordered_map<std::string, sf::Texture>  textures;
    std::unordered_map<std::string, TextureLevel> texture_levels;
};