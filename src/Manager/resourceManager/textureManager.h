#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>
#include "Common/animation.h"
// TextureManager 结构：
// 文件名->纹理资源

class TextureManager
{
public:
    /**
     * @brief 通过文件名获取纹理
     *
     * @param filename
     * @return const sf::Texture&
     */
    const sf::Texture& getTexture(const std::string& filename) const {
        auto it = textures.find(filename);
        if (it != textures.end()) {
            return it->second;
        }
        throw std::runtime_error("Texture not found: " + filename);
     }

    /**
     * @brief 通过文件名加载纹理
     *
     * @param filename
     * @param level 纹理资源级别
     */
    void LoadTexture(const std::string& filename, const TextureLevel level = TextureLevel::Common);

    /**
     * @brief 卸载临时纹理
     *
     */
    void UnloadSpecificTextures();

private:
    TextureMap  textures;
    std::unordered_map<std::string, TextureLevel> texture_levels;
};