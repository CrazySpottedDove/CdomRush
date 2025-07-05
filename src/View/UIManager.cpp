#include "UIManager.h"
#include "Common/action.h"
#include "Common/macros.h"
#include "Common/viewData.h"
#include "View/mapPosition.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <variant>

/**
 * @brief 保证传入的 frame_id 合法！
 *
 * @param view_data
 */
void UIManager::Render(const ViewData& view_data)
{
    if (view_data.animations->empty()) {
        ERROR("ViewData animations is empty.");
        return;
    }

    for (size_t layer_index = 0; layer_index < view_data.animations->size(); ++layer_index) {
        Animation&           animation = (*view_data.animations)[layer_index];
        if (!animation.hidden) {
            const AnimationGroup animation_group =
                animation_group_map->at(animation.prefix).at(animation.current_state);

            if (animation.last_state != animation.current_state ||
                (animation.current_state != State::Death && animation.frame_id > animation_group.to)) {
                animation.frame_id = animation_group.from;
                animation.pending  = true;
            }

            animation.last_state = animation.current_state;
            animation.last_state = animation.current_state;

            const SpriteFrameData& sprite_frame_data =
                sprite_frame_data_map->at(animation.prefix).at(animation.frame_id - 1);


            const sf::Texture& texture = texture_map->at(IMAGES_PATH + sprite_frame_data.textureName);

            sf::Sprite sprite(texture);

            sprite.setTextureRect(sprite_frame_data.frameRect);
            sprite.setTextureRect(sprite_frame_data.frameRect);

            sprite.setOrigin(Position(
                sprite_frame_data.displaySize.x * animation.anchor_x - sprite_frame_data.trim_left,
                sprite_frame_data.displaySize.y * ( animation.anchor_y) -
                    sprite_frame_data.trim_top));

            sprite.setScale(sf::Vector2f(animation.flip ? -animation.scale_x : animation.scale_x,
                                        animation.scale_y));

            sprite.setPosition(MapPosition(Position{view_data.position.x + animation.offset.x, view_data.position.y + animation.offset.y }));

            sprite.setRotation(sf::degrees(animation.rotation));


            window->draw(sprite);
            //else
                //INFO("Animation " + animation.prefix + " is hidden, not rendering.");
            ++animation.frame_id;

            if (animation.frame_id > animation_group.to) {
                animation.pending = false;
            }
        }

        // 对于第零层，处理action
        if (layer_index == 0 && animation.clicked) {
            for (size_t i = 0; i < animation.actions.size(); ++i) {
                switch (animation.actions[i].type) {
                case ActionType::SelectLevel:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    SUCCESS("Action: SelectLevel Triggered");
                    break;
                }
                case ActionType::CreateActionFx:
                {
                    animation.clicked = false;
                    Action create_action = animation.actions[i];
                    auto&  params        = std::get<CreateActionFxParams>(create_action.param);
                    params.position      = view_data.position + params.offset; // to be fixed
                    action_queue.push(create_action);
                    break;
                }
                case ActionType::UpgradeTower:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    SUCCESS("Action: UpgradeTower Triggered");
                    break;
                }
                case ActionType::CheckSoldierStatus:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    break;
                }
                case ActionType::CheckEnemyStatus:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    break;
                }
                case ActionType::CheckTowerStatus:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    break;
                }
                case ActionType::ChangeRallyPoint:
                {

                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    SUCCESS("Action: ChangeRallyPoint Triggered");
                    break;
                }
                case ActionType::SellTower:
                {
                    animation.clicked = false;
                    action_queue.push(animation.actions[i]);
                    SUCCESS("Action: SellTower Triggered");
                    break;
                }
                }
            }
        }
    }
}

void UIManager::RenderAll()
{
    for (const auto& view_data : *view_data_queue) {
        Render(view_data);
    }
}

void UIManager::ClearViewData()
{
    view_data_queue->clear();
    window                = nullptr;
    animation_group_map   = nullptr;
    sprite_frame_data_map = nullptr;
    texture_map           = nullptr;
}

/**
 * @brief 检查点击位置是否在ViewData检测边界内
 */
bool UIManager::IsClickHit(const ViewData& view_data, const sf::Vector2f& click_position) const
{
    if (!animation_group_map || !sprite_frame_data_map) {
        ERROR("Animation group map or sprite frame data map is not initialized.");
    }

    const Animation& animation = (*view_data.animations)[0];   // 只处理第0层

    const auto& animation_group =
        animation_group_map->at(animation.prefix).at(animation.current_state);
    const SpriteFrameData& sprite_frame_data =
        sprite_frame_data_map->at(animation.prefix).at(animation_group.to - 1);

    // 计算边界矩形...
    float left = view_data.position.x - sprite_frame_data.displaySize.x * animation.scale_x;
    float top  = view_data.position.y - sprite_frame_data.displaySize.y * animation.scale_y;

    sf::FloatRect bounds(MapPosition(sf::Vector2f(left, top)),
                         sf::Vector2f(2.0*sprite_frame_data.displaySize.x * animation.scale_x,
                                      2.0*sprite_frame_data.displaySize.y * animation.scale_y));

    return bounds.contains(click_position);
}

/**
 * @brief 处理SFML鼠标点击
 */
void UIManager::HandleClick()
{
    while (auto event_opt = window->pollEvent()) {
        if (!event_opt.has_value()) {
            continue;
        }
        const auto& event = event_opt.value();
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto& mouse_event = *event.getIf<sf::Event::MouseButtonPressed>();

            if (mouse_event.button == sf::Mouse::Button::Left) {

                // 遍历所有ViewData，寻找被点击对象
                bool hit_found = false;

                for (auto it = view_data_queue->begin(); it != view_data_queue->end(); ++it) {
                    const ViewData& view_data = *it;
                    if ((*view_data.animations)[0].actions.empty()) {
                        // if (view_data.animation->actions.empty()) {
                        continue;
                    }
                    sf::Vector2f click_position(static_cast<float>(mouse_event.position.x),
                                                static_cast<float>(mouse_event.position.y));
                    INFO("Click position: (" + std::to_string(click_position.x) + ", " +
                         std::to_string(click_position.y) + ")");

                    if (IsClickHit(view_data, click_position)) {
                        (*view_data.animations)[0].clicked = true;

                        // 如果有关联的actions，准备触发
                        INFO("Object has " +
                             std::to_string((*view_data.animations)[0].actions.size()) +
                             " actions available");

                        hit_found = true;
                        break;
                    }
                }

                if (!hit_found) {
                    INFO("No objects hit at click position");
                }
            }
        }
        action_queue.push(Action{ActionType::Delete, std::monostate{}});
    }
}