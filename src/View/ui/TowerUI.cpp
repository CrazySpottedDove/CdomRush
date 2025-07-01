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
    , last_layer_states_()
    , initialized_(false)
{
    if (tower_) {
        DEBUG_CODE(std::cout << "TowerUI: Created for tower at (" << tower_->position.x << "," << tower_->position.y
                  << ") with " << tower_->Layers.size() << " layers" << std::endl;)

        // 初始化图层上下文和状态缓存
        layer_contexts_.resize(tower_->Layers.size());
        last_layer_states_.resize(tower_->Layers.size(), State::Idle);

        DEBUG_CODE(std::cout << "TowerUI: Initialized " << layer_contexts_.size()
                  << " layer contexts for tower" << std::endl;)
    }
}

void TowerUI::Render(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    if (tower_ == nullptr || tower_->Layers.empty()) {
        DEBUG_CODE(std::cout << "TowerUI: Render skipped - null tower or no layers" << std::endl;)
        return;
    }

    if (!initialized_) {
        InitializeLayerAnimations();
    }

    UpdateLayerAnimations();

    // 渲染所有图层（按索引顺序，确保正确的视觉层次）
    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        RenderLayer(window, i, scale);
    }
}

void TowerUI::InitializeLayerAnimations()
{
    if (tower_ == nullptr || tower_->Layers.empty()) {
        return;
    }

    DEBUG_CODE(std::cout << "TowerUI: Initializing animations for " << tower_->Layers.size()
              << " layers" << std::endl;)

    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        const Layer& layer = tower_->Layers[i];
        AnimationContext& context = layer_contexts_[i];

        // 初始化图层动画上下文
        context.current_state = layer.animation.state;
        context.loop_enabled = true;  // 大多数Tower动画都是循环的
        context.is_paused = false;

        // 记录初始状态
        last_layer_states_[i] = layer.animation.state;

        DEBUG_CODE(std::cout << "TowerUI: Layer " << i << " initialized with prefix '"
                  << layer.animation.prefix << "' state " << static_cast<int>(layer.animation.state)
                  << " offset (" << layer.offset.x << "," << layer.offset.y << ")" << std::endl;)
    }

    initialized_ = true;
    DEBUG_CODE(std::cout << "TowerUI: All layer animations initialized" << std::endl;)
}

void TowerUI::UpdateLayerAnimations()
{
    if (tower_ == nullptr || tower_->Layers.empty() || !initialized_) {
        return;
    }

    bool any_layer_changed = false;

    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        const Layer& layer = tower_->Layers[i];
        AnimationContext& context = layer_contexts_[i];

        // 创建虚拟Entity
        Entity virtual_entity = CreateVirtualEntity(i);

        // 检查状态变化并播放新动画
        if (LayerNeedsAnimationUpdate(i)) {
            DEBUG_CODE(std::cout << "TowerUI: Layer " << i << " state changed from "
                      << static_cast<int>(last_layer_states_[i]) << " to "
                      << static_cast<int>(layer.animation.state) << std::endl;)

            // 根据状态确定循环播放设置:
            // 状态待确定。是不是这样呢？
            bool should_loop = true;
            if (layer.animation.state == State::Shoot || layer.animation.state == State::Attack ||
                layer.animation.state == State::ShootingUp || layer.animation.state == State::ShootingDown) {
                should_loop = false;  // 攻击动画通常不循环
            }

            // 播放新动画
            animation_player_.PlayAnimation(virtual_entity, context, layer.animation.state, should_loop);

            any_layer_changed = true;
        }

        // 更新图层动画帧（无论状态是否变化都要）
        animation_player_.Update(virtual_entity, context);
    }

    if (any_layer_changed) {
        SyncLayerStates();//脏标记，需要同步
        DEBUG_CODE(std::cout << "TowerUI: Layer states synchronized" << std::endl;)
    }
}

void TowerUI::RenderLayer(sf::RenderWindow& window, size_t layer_index, const sf::Vector2f& scale)
{
    if (layer_index >= tower_->Layers.size()) {
        DEBUG_CODE(std::cout << "TowerUI: Invalid layer index " << layer_index << std::endl;)
        return;
    }

    const AnimationContext& context = layer_contexts_[layer_index];

    // 虚拟Entity创建
    Entity virtual_entity = CreateVirtualEntity(layer_index);

    // 调用AnimationPlayer进行渲染
    animation_player_.Render(window, virtual_entity, context, scale);

    DEBUG_CODE(
        static int render_counter = 0;
        render_counter++;
        if (render_counter % 180 == 0) {  // 每3秒输出一次（假设60FPS）
            std::cout << "TowerUI: Rendered layer " << layer_index
                      << " at (" << virtual_entity.position.x << "," << virtual_entity.position.y
                      << ") prefix '" << virtual_entity.animation.prefix
                      << "' state " << static_cast<int>(virtual_entity.animation.state) << std::endl;
        }
    )
}

Entity TowerUI::CreateVirtualEntity(size_t layer_index) const
{
    if (layer_index >= tower_->Layers.size()) {
        DEBUG_CODE(std::cout << "TowerUI: CreateVirtualEntity - Invalid layer index " << layer_index << std::endl;)
        return Entity();  // 返回默认Entity
    }

    const Layer& layer = tower_->Layers[layer_index];

    Entity virtual_entity;
    virtual_entity.animation = layer.animation;
    // 计算最终渲染位置：Tower位置 + 图层偏移
    virtual_entity.position = tower_->position + layer.offset;

    return virtual_entity;
}

bool TowerUI::LayerNeedsAnimationUpdate(size_t layer_index) const
{
    if (layer_index >= tower_->Layers.size() || layer_index >= last_layer_states_.size()) {
        return false;
    }

    return last_layer_states_[layer_index] != tower_->Layers[layer_index].animation.state;
}

void TowerUI::SyncLayerStates()
{
    if (tower_ == nullptr || tower_->Layers.size() != last_layer_states_.size()) {
        return;
    }

    for (size_t i = 0; i < tower_->Layers.size(); ++i) {
        last_layer_states_[i] = tower_->Layers[i].animation.state;
    }

    DEBUG_CODE(std::cout << "TowerUI: Synchronized " << last_layer_states_.size()
              << " layer states" << std::endl;)
}