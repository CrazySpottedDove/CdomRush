#include "TowerUI.h"
#include <iostream>
#include "View/animation/animationPlayer.h"
#include "utils/macros.h"

/**
 * @brief 渲染Tower的所有图层 - 使用新的通用接口
 */
void TowerUI::Render(AnimationPlayer* animation_player, sf::RenderWindow& window, const Position& position, Animation& animation,const sf::Vector2f& scale)
{
    if (tower_ == nullptr || tower_->Layers.empty()) {
        DEBUG_CODE(std::cout << "TowerUI: Render skipped - null tower or no layers" << std::endl;)
        return;
    }

    //遍历每一层，对每一层独立渲染
    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        Layer& layer = tower_->Layers[i];
        const Position final_position = tower_->position + layer.offset;

        animation_player->RenderAnimation(window, final_position, layer.animation, scale);

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

void TowerUI::OnClick(Store& store, sf::RenderWindow& window){
    return;
}
