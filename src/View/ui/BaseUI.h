#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief UI基类 - 定义所有UI组件的通用接口
 */
class BaseUI
{
public:
    virtual ~BaseUI() = default;
    
    /**
     * @brief 渲染 - 所有UI类必须实现
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    virtual void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) = 0;
}; 