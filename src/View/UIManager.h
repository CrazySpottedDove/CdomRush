#pragma once
#include "Common/animation.h"
#include "Common/viewData.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <set>

class UIManager
{
public:
    UIManager() = default;

    /**
     * @brief 析构函数
     */
    ~UIManager() = default;

    void QueueViewData(const ViewData& view_data);
private:
    void Render(ViewData& view_data);

    struct ViewDataComparator
    {
        bool operator()(const ViewData& lhs, const ViewData& rhs) const
        {
            return lhs.position.y < rhs.position.y;
        }
    };
    sf::RenderWindow*                           window;
    AnimationGroupMap*                          animation_group_map = nullptr;
    SpriteFrameDataMap*                         sprite_frame_data_map = nullptr;
    TextureMap*                                 texture_map = nullptr;
    std::multiset<ViewData, ViewDataComparator> view_data_queue;
};