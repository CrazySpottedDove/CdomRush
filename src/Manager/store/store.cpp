#include "Manager/store/store.h"
#include "Function/calc/damage.h"
#include "Function/calc/hp.h"
#include "Function/calc/motion.h"
#include "Model/components/damage.h"
#include "Model/enemies/enemies.h"
#include "Model/soldiers/soldiers.h"
#include "utils/macros.h"
#include <algorithm>
#include <chrono>
#include <thread>

// 核心：指针要么有效，要么为 nullptr，不可以有悬垂引用

// 在每一次 Update 周期中，首先，执行所有的 Update 事件。其中，生命周期结束的变量进入 entities_to_delete 集合中，并不立刻删除。因此，此时它们的指针依旧有效。

// 在所有的 Update 事件结束后，我们遍历所有的实体，将其中持有的所有存在于 entities_to_delete 集合中的指针置为 nullptr。

// 最后，我们遍历 entities_to_delete 集合，删除其中的所有实体。

void Store::UpdateGameLogic()
{
    // // 处理伤害事件
    // damage_events.erase(std::remove_if(
    //     damage_events.begin(), damage_events.end(), [this](DamageEvent& damage_event) -> bool {
    //         if (damage_event.target == nullptr || calc::is_dead(*damage_event.target)) {
    //             return true;
    //         }
    //         if (damage_event.data.apply_delay > 0) {
    //             // 减少延迟计数
    //             --damage_event.data.apply_delay;
    //             return false;   // 保留事件
    //         }

    //         // 延迟为0，执行伤害
    //         calc::enforce_damage(damage_event);
    //         return true;   // 移除已执行的事件
    //     }));
}


void Store::Update()
{
    // const auto new_damage_event_end = std::remove_if(
    //     damage_events.begin(), damage_events.end(), [](DamageEvent& damage_event) -> bool {
    //         // 检查目标是否有效且存活
    //         if (damage_event.target == nullptr || calc::is_dead(*damage_event.target)) {
    //             return true;   // 移除无效事件
    //         }

    //         if (damage_event.data.apply_delay > 0) {
    //             // 减少延迟计数
    //             --damage_event.data.apply_delay;
    //             return false;   // 保留事件
    //         }

    //         // 延迟为0，执行伤害
    //         calc::enforce_damage(damage_event);
    //         return true;   // 移除已执行的事件
    //     });

    // // 删除已处理的事件
    // damage_events.erase(new_damage_event_end, damage_events.end());

    // // 执行所有敌人的更新事件
    // const auto new_enemy_end =
    //     std::remove_if(enemies.begin(), enemies.end(), [this](Enemy* enemy) -> bool {
    //         if (calc::is_dead(*enemy) && calc::should_remove(*this, *enemy)) {
    //             delete enemy;
    //             return true;   // 删除敌人
    //         }
    //         if (calc::enemy_reached_defence_point(*this, *enemy)) {
    //             life -= enemy->life_cost;
    //             gold += enemy->gold;
    //             delete enemy;
    //             return true;
    //         }
    //         enemy->Update(*this);
    //         return false;   // 保留活着的敌人
    //     });
    // enemies.erase(new_enemy_end, enemies.end());

    // const auto new_soldier_end =
    //     std::remove_if(soldiers.begin(), soldiers.end(), [this](Soldier* soldier) -> bool {
    //         if (calc::is_dead(*soldier) && calc::should_remove(*this, *soldier)) {
    //             delete soldier;
    //             return true;   // 删除士兵
    //         }
    //         soldier->Update(*this);
    //         return false;   // 保留活着的士兵
    //     });

    // soldiers.erase(new_soldier_end, soldiers.end());
}

void Store::Game()
{
    static constexpr int FRAME_LENGTH_IN_MILLISECONDS = FRAME_LENGTH * 1000;
    while (true) {
        switch (game_state) {
        case GameState::Begin:
            // AnimationPlayer::DrawTotalMap();

            // AnimationPlayer::
            break;
        case GameState::GameStart:
            time = 0.0;
            while (true) {
                Update();
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(FRAME_LENGTH_IN_MILLISECONDS));
                time += FRAME_LENGTH;
            }
            break;
        case GameState::GamePlaying:
            while (true) {
                Update();
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(FRAME_LENGTH_IN_MILLISECONDS));
                time += FRAME_LENGTH;
            }
        case GameState::GameOver:
            // AnimationPlayer::DrawGameOver();
            break;
        case GameState::Loading:
            // AnimationPlayer::DrawLoading();
            break;
        }
    }
}

Store::Store() {}