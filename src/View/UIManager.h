#pragma once
#include "Common/animation.h"
#include "Common/viewData.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>

class UIManager
{
public:
    UIManager() = default;

    /**
     * @brief 析构函数
     */
    ~UIManager() = default;

    void Render(ViewData& view_data);

private:
    sf::RenderWindow* window;
    AnimationGroupMap* animation_group_map;
    SpriteFrameDataMap* sprite_frame_data_map;
    TextureMap* texture_map;
};