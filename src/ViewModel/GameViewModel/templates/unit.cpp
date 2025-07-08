#include "ViewModel/GameViewModel/templates/unit.h"
#include "Common/macros.h"
#include "Common/state.h"
#include "Common/renderData.h"
#include "ViewModel/GameViewModel/store/store.h"

bool Unit::OnDamage()
{
    return true;
};
bool Unit::is_moving()
{
    if (animations[0].current_state == State::WalkingLeftRight ||
        animations[0].current_state == State::WalkingUp ||
        animations[0].current_state == State::WalkingDown ||
        animations[0].current_state == State::Running) {
        return true;   // 如果状态是左右行走、向上行走、向下行走或奔跑，返回 true
    }
    return false;   // 否则返回 false
}
State Unit::walkjudge()
{
    if (heading == Heading::Right)
        return State::WalkingLeftRight;   // 如果方向是向右，设置状态为左右行走
    else if (heading == Heading::Up)
        return State::WalkingUp;   // 如果方向是向上，设置状态为向上行走
    else if (heading == Heading::Down)
        return State::WalkingDown;   // 如果方向是向下，设置状态为向下行走
    else if(heading == Heading::Left)
        return State::WalkingLeftRight;   // 如果方向是向左，设置状态为左右行走
    else
        return State::Running;
}

void Unit::QueueViewData(Store& store)
{
    auto& render_data_queue = *store.GetViewDataQueue();
    render_data_queue.emplace(RenderData(&animations, position, COMMON_LAYER));
    if(health.hp > 0){
        render_data_queue.emplace(RenderData(health.hp / health.hp_max, position + health_bar_offset));
    }
}