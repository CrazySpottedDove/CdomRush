#include "ViewModel/GameViewModel/fx/fx.h"
#include "ViewModel/GameViewModel/components/state.h"

LevelFlag::LevelFlag(){
    animation.prefix = "map_flag";
    animation.current_state = State::Idle;
    animation.anchor_x = 0.0;
    animation.anchor_y = 0.0;
}

Map::Map(){
    animation.prefix = "map_background";
    animation.current_state = State::Idle;
    animation.anchor_x = 0.0;
    animation.anchor_y = 0.0;
}

Map::Map(const std::string& prefix) {
    animation.prefix = prefix;
    animation.current_state = State::Idle;
    animation.anchor_x = 0.0;
    animation.anchor_y = 0.0;
}