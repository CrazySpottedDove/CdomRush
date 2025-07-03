#include "ViewModel/GameViewModel/components/attack.h"
#include "ViewModel/GameViewModel/store/store.h"

bool Attack::IsReady(const Store& store) const noexcept{
    return (store.time - last_time) >= cooldown;
}

void Attack::SetLastTime(const double time) noexcept {
    last_time = time;
}