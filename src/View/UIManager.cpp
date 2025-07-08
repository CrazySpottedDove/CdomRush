#include "UIManager.h"
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/renderData.h"
#include "View/mapPosition.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <variant>

/**
 * @brief 保证传入的 frame_id 合法！
 *
 * @param render_data
 */
void UIManager::Render(const RenderData& render_data)
{
    switch (render_data.type) {
    case RenderDataType::Animation:
    {
        for (size_t layer_index = 0; layer_index < render_data.animations->size(); ++layer_index) {
            Animation& animation = (*render_data.animations)[layer_index];
            if (!animation.hidden) {
                // INFO("Render Animation: " + animation.prefix + " at layer " +
                //      std::to_string(layer_index) + ", state: " +
                //      std::to_string(static_cast<int>(animation.current_state)) +
                //      ", frame_id: " + std::to_string(animation.frame_id));
                const AnimationGroup animation_group =
                    animation_group_map->at(animation.prefix).at(animation.current_state);

                if (animation.last_state != animation.current_state ||
                    (animation.current_state != State::Death &&
                     animation.frame_id > animation_group.to)) {
                    animation.frame_id = animation_group.from;
                    animation.pending  = true;
                }

                // 死亡结束后，一直播放死亡的最后一帧
                if (animation.current_state == State::Death && animation.pending == false &&
                    animation.last_state == State::Death) {
                    animation.frame_id = animation_group.to;
                }

                animation.last_state = animation.current_state;

                const SpriteFrameData& sprite_frame_data =
                    sprite_frame_data_map->at(animation.prefix).at(animation.frame_id - 1);

                const sf::Texture& texture =
                    texture_map->at(IMAGES_PATH + sprite_frame_data.textureName);

                sf::Sprite sprite(texture);

                sprite.setTextureRect(sprite_frame_data.frameRect);

                sprite.setOrigin(Position(sprite_frame_data.displaySize.x * animation.anchor_x -
                                              sprite_frame_data.trim_left,
                                          sprite_frame_data.displaySize.y * (animation.anchor_y) -
                                              sprite_frame_data.trim_top));

                sprite.setScale(sf::Vector2f(
                    animation.flip ? -animation.scale_x : animation.scale_x, animation.scale_y));

                sprite.setPosition(
                    MapPosition(Position{render_data.position.x + animation.offset.x,
                                         render_data.position.y + animation.offset.y}));

                sprite.setRotation(sf::degrees(animation.rotation));

                window->draw(sprite);

                ++animation.frame_id;

                if (animation.frame_id > animation_group.to) {
                    animation.pending = false;
                }
            }
        }
        break;
    }
    case RenderDataType::HealthBar:
    {
        const float bar_width        = 32.0f;
        const float bar_height       = 4.0f;
        const float border_thickness = 1.0f;

        // 血条位置（在实体上方）
        const float bar_x = render_data.position.x - bar_width / 2.0f;
        const float bar_y = render_data.position.y;   // 在实体上方20像素

        // 将位置转换为屏幕坐标
        const sf::Vector2f screen_pos = MapPosition(Position{bar_x, bar_y});

        // 1. 绘制背景（红色）
        sf::RectangleShape background;
        background.setSize(sf::Vector2f(bar_width, bar_height));
        background.setPosition(screen_pos);
        background.setFillColor(sf::Color::Red);
        window->draw(background);

        // 2. 绘制血量（绿色）
        if (render_data.health_rate > 0.0f) {
            sf::RectangleShape health_bar;
            health_bar.setSize(sf::Vector2f(bar_width * render_data.health_rate, bar_height));
            health_bar.setPosition(screen_pos);
            health_bar.setFillColor(sf::Color::Green);
            window->draw(health_bar);
        }

        // 3. 绘制边框（黑色）
        sf::RectangleShape border;
        border.setSize(sf::Vector2f(bar_width, bar_height));
        border.setPosition(screen_pos);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineThickness(border_thickness);
        border.setOutlineColor(sf::Color::Black);
        window->draw(border);
        break;
    }
    case RenderDataType::Text:
    {
        sf::Text text(*font, render_data.text, 15);
        text.setPosition(MapPosition(render_data.position));
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        window->draw(text);
        break;
    }
    case RenderDataType::TowerRange:{
        // 将世界坐标转换为屏幕坐标
        sf::Vector2f screen_pos = MapPosition(render_data.position);

        // 创建圆形（可以设置为椭圆）
        sf::CircleShape range_circle;
        range_circle.setRadius(render_data.range);

        // 设置圆心位置
        range_circle.setPosition(Position(screen_pos.x - render_data.range, screen_pos.y - render_data.range));

        // 设置颜色和样式
        range_circle.setFillColor(sf::Color(0, 0, 255, 80));   // 蓝色，透明度80/255

        // ✅ 设置边框颜色（稍微不透明一些）
        range_circle.setOutlineThickness(2.0f);
        range_circle.setOutlineColor(sf::Color(0, 150, 255, 150));   // 青蓝色边框，透明度150/255

        // 如果要椭圆效果，可以设置缩放
        range_circle.setScale(sf::Vector2f(1.0f, 0.7f));  // 水平方向正常，垂直方向压缩

        window->draw(range_circle);
        break;
    }
    }
}

void UIManager::PrecessUI()
{
    window->clear();
    for (auto it = render_data_queue->begin(); it != render_data_queue->end(); ++it) {
        Render(*it);
    }
    window->display();
    HandleClick();
    render_data_queue->clear();
}

void UIManager::ClearViewData()
{
    render_data_queue->clear();
    window                = nullptr;
    animation_group_map   = nullptr;
    sprite_frame_data_map = nullptr;
    texture_map           = nullptr;
}

/**
 * @brief 检查点击位置是否在ViewData检测边界内
 */
bool UIManager::IsClickHit(const RenderData& render_data, const sf::Vector2f& click_position) const
{
    if (!animation_group_map || !sprite_frame_data_map) {
        ERROR("Animation group map or sprite frame data map is not initialized.");
    }
    if(render_data.type == RenderDataType::TowerRange){
        const Position click_position_back = MapPositionBack(click_position);

        // 计算椭圆的中心点
        float center_x = render_data.position.x;
        float center_y = render_data.position.y;

        // 椭圆的半径（长轴和短轴）
        float radius_x = render_data.range;          // 水平半径
        float radius_y = render_data.range * 0.7f;   // 垂直半径（对应 scale 0.7f）

        // 计算点到椭圆中心的距离
        float dx = click_position_back.x - center_x;
        float dy = click_position_back.y - center_y;

        // 椭圆内部检测公式：(x/a)² + (y/b)² <= 1
        // 其中 a 是水平半径，b 是垂直半径
        float ellipse_equation =
            (dx * dx) / (radius_x * radius_x) + (dy * dy) / (radius_y * radius_y);

        INFO("Check Bounds With Tower Range: " + std::to_string(render_data.range));
        INFO("Click position back: (" + std::to_string(click_position_back.x) + ", " +
             std::to_string(click_position_back.y) + ")");
        INFO("Ellipse center: (" + std::to_string(center_x) + ", " + std::to_string(center_y) +
             ")");
        INFO("Ellipse radii: (" + std::to_string(radius_x) + ", " + std::to_string(radius_y) + ")");
        INFO("Ellipse equation result: " + std::to_string(ellipse_equation));

        // 如果椭圆方程结果 <= 1，则点在椭圆内
        return ellipse_equation <= 1.0f;
    }
    const Animation& animation =
        (*render_data.animations)[(*render_data.animations)[0].hidden ? 1 : 0];   // 只处理第0层

    const auto& animation_group =
        animation_group_map->at(animation.prefix).at(animation.current_state);
    const SpriteFrameData& sprite_frame_data =
        sprite_frame_data_map->at(animation.prefix).at(animation_group.to - 1);

    // 计算边界矩形...
    const Position before_map_position =
        Position{render_data.position.x + animation.offset.x -
                     sprite_frame_data.displaySize.x * animation.anchor_x * animation.scale_x,
                 render_data.position.y + animation.offset.y -
                     sprite_frame_data.displaySize.y * animation.anchor_y * animation.scale_y};

    sf::FloatRect bounds(before_map_position,
                         sf::Vector2f(sprite_frame_data.displaySize.x * animation.scale_x,
                                      sprite_frame_data.displaySize.y * animation.scale_y));

    const Position click_position_back = MapPositionBack(click_position);
    INFO("Check Bounds With Prefix: " + animation.prefix);
    INFO("Click position back: (" + std::to_string(click_position_back.x) + ", " +
         std::to_string(click_position_back.y) + ")");
    INFO("Bounds: (" + std::to_string(bounds.position.x) + ", " +
             std::to_string(bounds.position.y) + ") - (" +
             std::to_string(bounds.position.x + bounds.size.x) + ", " +
             std::to_string(bounds.position.y + bounds.size.y) + ")"
         << std::endl);
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
        if(event.is<sf::Event::Closed>()){
            window->close();
            return;
        }
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto&    mouse_event = *event.getIf<sf::Event::MouseButtonPressed>();
            const Position click_position{static_cast<float>(mouse_event.position.x),
                                          static_cast<float>(mouse_event.position.y)};
            INFO("Click position: (" + std::to_string(click_position.x) + ", " +
                 std::to_string(click_position.y) + ")");
            if (mouse_event.button == sf::Mouse::Button::Left) {
                action_queue.push(Action{ActionType::Delete, std::monostate{}});
                // 遍历所有ViewData，寻找被点击对象
                bool hit_found = false;

                for (auto it = render_data_queue->rbegin(); it != render_data_queue->rend(); ++it) {
                    const RenderData& render_data = *it;
                    if (render_data.type == RenderDataType::Text) {
                        continue;
                    }
                    if(render_data.animations == nullptr || render_data.animations->empty()) {
                        continue;
                    }
                    if ((*render_data.animations)[0].actions.empty()) {
                        continue;
                    }

                    if (IsClickHit(render_data, click_position)) {
                        // (*render_data.animations)[0].clicked = true;
                        for (size_t i = 0; i < (*render_data.animations)[0].actions.size(); ++i) {
                            if(render_data.type == RenderDataType::TowerRange){
                                std::get<ChangeRallyPointParams>((*render_data.animations)[0].actions[i].param).new_rally_point = MapPositionBack(click_position);
                            }
                            action_queue.push((*render_data.animations)[0].actions[i]);
                        }
                        hit_found = true;
                        break;
                    }
                }

                if (!hit_found) {
                    WARNING("No objects hit at click position");
                }
                else {
                    SUCCESS("Object hit at click position");
                }
            }
            else if (mouse_event.button == sf::Mouse::Button::Right) {
                // 右键点击，可能是取消选择或其他操作
                action_queue.push(Action{ActionType::EndGameStart, std::monostate{}});
            }
        }
    }
}