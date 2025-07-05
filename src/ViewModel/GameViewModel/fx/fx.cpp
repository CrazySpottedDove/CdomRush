#include "ViewModel/GameViewModel/fx/fx.h"
#include "Common/animation.h"
#include "Common/state.h"


LevelFlag::LevelFlag(){
    animations.emplace_back(Animation(State::Idle,"map_flag"));
}

Map::Map(){
    animations.emplace_back(Animation(State::Idle,"map_background"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 1.0;
    position.x = -157.5;
    position.y = 0.0;
}

CommonUpgradeIcon::CommonUpgradeIcon(){
    // animations.emplace_back(Animation(State::Enabled,))
}