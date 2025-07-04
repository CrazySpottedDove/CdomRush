#include "UIManager.h"
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/viewData.h"
#include "View/mapPosition.h"
#include "ViewModel/GameViewModel/fx/fx.h"
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
            // INFO("Rendering animation: " + animation.prefix + " at layer " +
            //      std::to_string(layer_index) + ", current state: " +
            //      std::to_string(static_cast<int>(animation.current_state)) +
            //      ", frame ID: " + std::to_string(animation.frame_id));
            const AnimationGroup animation_group =
                animation_group_map->at(animation.prefix).at(animation.current_state);

            if (animation.last_state != animation.current_state ||
                (animation.current_state != State::Death && animation.frame_id > animation_group.to)) {
                animation.frame_id = animation_group.from;
                animation.pending  = true;
            }

            // 死亡结束后，一直播放死亡的最后一帧
            if(animation.current_state == State::Death && animation.pending == false && animation.last_state == State::Death){
                animation.frame_id = animation_group.to;
            }

            animation.last_state = animation.current_state;

            const SpriteFrameData& sprite_frame_data =
                sprite_frame_data_map->at(animation.prefix).at(animation.frame_id - 1);

            const sf::Texture& texture = texture_map->at(IMAGES_PATH + sprite_frame_data.textureName);

            sf::Sprite sprite(texture);

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
                    action_queue.push(animation.actions[i]);
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

    const Animation& animation = (*view_data.animations)[(*view_data.animations).size() == 1 ? 0 : 1];   // 只处理第0层

    const auto& animation_group =
        animation_group_map->at(animation.prefix).at(animation.current_state);
    const SpriteFrameData& sprite_frame_data =
        sprite_frame_data_map->at(animation.prefix).at(animation_group.to - 1);

    // 计算边界矩形...
    const Position before_map_position =
        Position{view_data.position.x + animation.offset.x -
                     sprite_frame_data.displaySize.x * animation.anchor_x * animation.scale_x,
                 view_data.position.y + animation.offset.y -
                     sprite_frame_data.displaySize.y * animation.anchor_y * animation.scale_y};

    sf::FloatRect bounds(before_map_position,
                         sf::Vector2f(sprite_frame_data.displaySize.x * animation.scale_x,
                                      sprite_frame_data.displaySize.y * animation.scale_y));

    const Position click_position_back = MapPositionBack(click_position);
    INFO("Check Bounds With Prefix: " + animation.prefix);
    INFO("Click position back: (" + std::to_string(click_position_back.x) + ", " +
         std::to_string(click_position_back.y) + ")");
    INFO("Bounds: (" + std::to_string(bounds.position.x) + ", " + std::to_string(bounds.position.y) + ") - (" +
         std::to_string(bounds.position.x + bounds.size.x) + ", " +
         std::to_string(bounds.position.y + bounds.size.y) + ")" << std::endl);
    return bounds.contains(click_position_back);
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
            const Position click_position{
                static_cast<float>(mouse_event.position.x),
                static_cast<float>(mouse_event.position.y)};
            INFO("Click position: (" + std::to_string(click_position.x) + ", " +
                 std::to_string(click_position.y) + ")");
            if (mouse_event.button == sf::Mouse::Button::Left) {

                // 遍历所有ViewData，寻找被点击对象
                bool hit_found = false;

                for (auto it = view_data_queue->begin(); it != view_data_queue->end(); ++it) {
                    const ViewData& view_data = *it;
                    if ((*view_data.animations)[0].actions.empty()) {
                        continue;
                    }

                    if (IsClickHit(view_data, click_position)) {
                        (*view_data.animations)[0].clicked = true;
                        hit_found = true;
                        break;
                    }
                }

                if (!hit_found) {
                    WARNING("No objects hit at click position");
                }else{
                    SUCCESS("Object hit at click position");
                }
            }
            action_queue.push(Action{ActionType::Delete, std::monostate{}});
        }
    }
}