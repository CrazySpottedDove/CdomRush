#include "Model/fx/fx.h"
#include "Model/components/state.h"

LevelFlag::LevelFlag(){
    animation.prefix = "map_flag";
    animation.current_state = State::Idle;
}