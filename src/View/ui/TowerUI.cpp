#include "TowerUI.h"
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
TowerUI::TowerUI(Tower* tower, AnimationPlayer& animation_player)
    : tower_(tower)
    , animation_player_(animation_player)
    , layer_contexts_()
    //, last_layer_states_()
    //, initialized_(false)
{
    if (tower_) {
        DEBUG_CODE(std::cout << "TowerUI: Created for tower at (" << tower_->position.x << "," << tower_->position.y
                  << ") with " << tower_->Layers.size() << " layers" << std::endl;)

        // 每一层创建独立的AnimationContext
        layer_contexts_.resize(tower_->Layers.size());
        //last_layer_states_.resize(tower_->Layers.size(), State::Idle);

        DEBUG_CODE(std::cout << "TowerUI: Initialized " << layer_contexts_.size()
                  << " layer contexts for tower" << std::endl;)
    }
}

/**
 * @brief 渲染Tower的所有图层 - 使用新的通用接口
 */
void TowerUI::Render(sf::RenderWindow& window, const Position& position, Animation& animation,const sf::Vector2f& scale)
{
    if (tower_ == nullptr || tower_->Layers.empty()) {
        DEBUG_CODE(std::cout << "TowerUI: Render skipped - null tower or no layers" << std::endl;)
        return;
    }

    //遍历每一层，对每一层独立渲染
    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        Layer& layer = tower_->Layers[i];
        AnimationContext& context = layer_contexts_[i];
        Position final_position = tower_->position + layer.offset;
        animation_player_.UpdateAnimation(layer.animation, context);

        animation_player_.RenderAnimation(window, final_position, layer.animation, context, scale);

        DEBUG_CODE(
            static int render_counter = 0;
            render_counter++;
            if (render_counter % 300 == 0) {
                std::cout << "TowerUI: Rendered layer " << i << " for tower at (" 
                          << final_position.x << "," << final_position.y 
                          << ") with animation prefix: " << layer.animation.prefix 
                          << ", state: " << static_cast<int>(layer.animation.state) 
                          << std::endl;
            }
        )
    }
}


        

 