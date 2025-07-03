#include "UIManager.h"
#include "Common/action.h"
#include "Common/macros.h"
#include "Common/viewData.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

/**
 * @brief 保证传入的 frame_id 合法！
 *
 * @param view_data
 */
void UIManager::Render(const ViewData& view_data)
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
    if( animation.clicked && !animation.actions.empty()) {
        Action action = animation.actions.front();

        // 处理不同的 Action 类型
        switch (action.type) {
            case ActionType::SelectLevel: {
                animation.clicked = false; // 对于选关卡，点击后UI不再处于点击状态
                const auto& params = std::get<SelectLevelParams>(action.param);
                action_queue.push(action);
                INFO("Selected level: " + params.level_name);
            }
            //case..
        }

        animation.actions.erase(animation.actions.begin()); // 移除已处理的 Action
    }
}

void UIManager::QueueViewData(ViewData view_data)
{
    view_data_queue.insert(view_data);
}

void UIManager::RenderAll()
{
    for (const auto& view_data : view_data_queue) {
        Render(view_data);
    }
}

void UIManager::ClearViewData()
{
    view_data_queue.clear();
    window = nullptr;
    animation_group_map = nullptr;
    sprite_frame_data_map = nullptr;
    texture_map = nullptr;
}

/**
 * @brief 检查点击位置是否在ViewData检测边界内
 */
bool UIManager::IsClickHit(const ViewData& view_data, const sf::Vector2f& click_position) const
{
    if (!animation_group_map || !sprite_frame_data_map) {
        ERROR("Animation group map or sprite frame data map is not initialized.");
    }

    const Animation& animation = *view_data.animation;
    
    const auto& animation_group = animation_group_map->at(animation.prefix).at(animation.current_state);
    const SpriteFrameData& sprite_frame_data = sprite_frame_data_map->at(animation.prefix).at(animation.frame_id);
    
    // 实际渲染尺寸
    float actual_width = sprite_frame_data.displaySize.x * std::abs(animation.scale_x);
    float actual_height = sprite_frame_data.displaySize.y * animation.scale_y;
    
    // 计算锚点偏移
    float anchor_offset_x = actual_width * animation.anchor_x;
    float anchor_offset_y = actual_height * (1.0f - animation.anchor_y);
    
    // 计算边界矩形（世界坐标）
    float left = view_data.position.x - anchor_offset_x;
    float top = view_data.position.y - anchor_offset_y;

    sf::FloatRect bounds(sf::Vector2f(left, top),sf::Vector2f( actual_width, actual_height));
    return bounds.contains(click_position);
}

/**
 * @brief 处理SFML鼠标点击
 */
void UIManager::HandleClick(const sf::Event& event, const sf::RenderWindow& window, const sf::Vector2f& click_position)
{
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouse_event = *event.getIf<sf::Event::MouseButtonPressed>();
        
        if (mouse_event.button == sf::Mouse::Button::Left) {
            // 将鼠标屏幕坐标转换为世界坐标
            sf::Vector2f world_position = window.mapPixelToCoords(mouse_event.position);
            
            INFO("Mouse clicked at screen (" + std::to_string(mouse_event.position.x) + 
                 ", " + std::to_string(mouse_event.position.y) + 
                 ") -> world (" + std::to_string(world_position.x) + 
                 ", " + std::to_string(world_position.y) + ")");
            
            // 遍历所有ViewData，寻找被点击对象
            // 注意：由于使用multiset且按y坐标排序，我们需要转换为可修改的容器
            std::vector<ViewData> temp_view_data(view_data_queue.begin(), view_data_queue.end());
            bool hit_found = false;
            
            for (auto it = temp_view_data.begin(); it != temp_view_data.end(); ++it) {
                ViewData& view_data = *it;
                
                if (IsClickHit(view_data, click_position)) {
                    INFO("Hit detected on object at (" + std::to_string(view_data.position.x) + 
                        ", " + std::to_string(view_data.position.y) + ")");
                    
                    // 标记为被点击
                    view_data.animation->clicked = true;
                    
                    // 如果有关联的actions，准备触发
                    if (!view_data.animation->actions.empty()) {
                        INFO("Object has " + std::to_string(view_data.animation->actions.size()) + " actions available");
                    }
                    
                    hit_found = true;
                    break; 
                }
            }
            
            if (!hit_found) {
                INFO("No objects hit at click position");
            }
    
            // 更新view_data_queue（由于我们修改了数据，需要重新构建）
            view_data_queue.clear();
            for (const auto& view_data : temp_view_data) {
                view_data_queue.insert(view_data);
            }
        }
    }
}