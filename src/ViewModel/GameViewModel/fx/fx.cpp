#include "ViewModel/GameViewModel/fx/fx.h"
#include "ViewModel/GameViewModel/components/state.h"

LevelFlag::LevelFlag(){
    animation.prefix = "map_flag";
    animation.current_state = State::Idle;
}