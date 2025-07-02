#include "UIManager.h"
#include "Common/macros.h"
#include "Common/viewData.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

/**
 * @brief 保证传入的 frame_id 合法！
 *
 * @param view_data
 */
void UIManager::Render(ViewData& view_data)
{
    Animation&             animation         = *view_data.animation;
    const AnimationGroup  animation_group   = animation_group_map->at(animation.prefix).at(animation.current_state);

    if (animation.last_state != animation.current_state ||
        (animation.current_state != State::Death && animation.frame_id > animation_group.to)) {
        animation.frame_id = animation_group.from;
        animation.pending  = true;
    }

    animation.last_state = animation.current_state;

    const SpriteFrameData& sprite_frame_data = sprite_frame_data_map->at(animation.prefix).at(animation.frame_id);

    const sf::Texture& texture = texture_map->at(sprite_frame_data.textureName);

    sf::Sprite sprite(texture);

    sprite.setTextureRect(sprite_frame_data.frameRect);

    sprite.setOrigin(Position(
        sprite_frame_data.displaySize.x * animation.anchor_x - sprite_frame_data.trim_left,
        sprite_frame_data.displaySize.y * (1 - animation.anchor_y) - sprite_frame_data.trim_top));

    sprite.setScale(sf::Vector2f(animation.flip ? -animation.scale_x:animation.scale_x, animation.scale_y));

    sprite.setPosition(view_data.position);

    window->draw(sprite);

    ++animation.frame_id;

    if(animation.frame_id > animation_group.to){
        animation.pending = false;
    }

    // TODO: 处理 Action 绘画
}
