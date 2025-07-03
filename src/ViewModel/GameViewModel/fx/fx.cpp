#include "ViewModel/GameViewModel/fx/fx.h"


LevelFlag::LevelFlag(){
    animation.prefix = "map_flag";
    animation.current_state = State::Idle;
    // animation.anchor_x = 0.0;
    // animation.anchor_y = 1.0;
}

Map::Map(){
    animation.prefix = "map_background";
    animation.current_state = State::Idle;
    animation.anchor_x = 0.0;
    animation.anchor_y = 1.0;
    position.x = 0.0;
    position.y = 0.0;
}

Map::Map(const std::string& prefix) {
    animation.prefix = prefix;
    animation.current_state = State::Idle;
    animation.anchor_x = 0.0;
    animation.anchor_y = 1.0;
    position.x = 0.0;
    position.y = 0.0;
}