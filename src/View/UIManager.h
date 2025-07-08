#pragma once
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/renderData.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <set>
#include <queue>

class UIManager
{
public:
    UIManager() = default;

    /**
     * @brief 析构函数
     */
    ~UIManager() = default;

    /**
     * @brief 确保所有的 ViewData 都进入 render_data_queue 中后使用，处理所有的渲染和点击事件。
     *
     */
    void PrecessUI();
    void ClearViewData();
    void SetViewData(std::multiset<RenderData, RenderDataComparator>& render_data_queue)
    {
        this->render_data_queue = &render_data_queue;
    }
    void SetWindow(sf::RenderWindow& window)
    {
        this->window = &window;
    }
    void SetAnimationGroupMap(AnimationGroupMap& animation_group_map)
    {
        this->animation_group_map = &animation_group_map;
    }
    void SetSpriteFrameDataMap(SpriteFrameDataMap& sprite_frame_data_map)
    {
        this->sprite_frame_data_map = &sprite_frame_data_map;
    }
    void SetTextureMap(TextureMap& texture_map)
    {
        this->texture_map = &texture_map;
    }

    void SetFont(sf::Font* font){
        this->font = font;
    }
    std::queue<Action>* GetActionQueue()
    {
        return &action_queue;
    }

private:
    void Render(const RenderData& render_data);

    /**
     * @brief 处理SFML鼠标点击
     * @param event SFML事件对象
     * @param window 渲染窗口（用于坐标转换）
     * @param click_position 鼠标点击的世界坐标位置
     */
    void HandleClick();

    /**
     * @brief 检查点击位置是否在ViewData的边界内
     * @param click_position 点击位置
     */
    bool IsClickHit(const RenderData& render_data, const sf::Vector2f& click_position) const;

    // struct ViewDataComparator
    // {
    //     bool operator()(const ViewData& lhs, const ViewData& rhs) const
    //     {
    //         return lhs.position.y < rhs.position.y;
    //     }
    // };
    sf::RenderWindow*                           window = nullptr;
    AnimationGroupMap*                          animation_group_map = nullptr;
    SpriteFrameDataMap*                         sprite_frame_data_map = nullptr;
    TextureMap*                                 texture_map = nullptr;
    std::multiset<RenderData, RenderDataComparator>* render_data_queue;
    std::queue<Action>                          action_queue;
    sf::Font* font;
};