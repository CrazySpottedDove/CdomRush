#pragma once
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/viewData.h"
#include <SFML/Graphics.hpp>
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

    void QueueViewData(ViewData view_data);

    void RenderAll();
    void ClearViewData();

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

    Action GetNextAction()
    {
        if (action_queue.empty())
            return Action();

        Action action = action_queue.front();
        action_queue.pop();
        return action;
    }

    /**
     * @brief 处理SFML鼠标点击
     * @param event SFML事件对象
     * @param window 渲染窗口（用于坐标转换）
     * @param click_position 鼠标点击的世界坐标位置
     */
    void HandleClick(const sf::Event& event, const sf::RenderWindow& window, const sf::Vector2f& click_position);

private:
    void Render(const ViewData& view_data);

    /**
     * @brief 检查点击位置是否在ViewData的边界内
     * @param click_position 点击位置
     */
    bool IsClickHit(const ViewData& view_data, const sf::Vector2f& click_position) const;

    struct ViewDataComparator
    {
        bool operator()(const ViewData& lhs, const ViewData& rhs) const
        {
            return lhs.position.y < rhs.position.y;
        }
    };
    sf::RenderWindow*                           window = nullptr;
    AnimationGroupMap*                          animation_group_map = nullptr;
    SpriteFrameDataMap*                         sprite_frame_data_map = nullptr;
    TextureMap*                                 texture_map = nullptr;
    std::multiset<ViewData, ViewDataComparator> view_data_queue;
    std::queue<Action>                          action_queue;
};