#pragma once

#include "ViewModel/GameViewModel/templates/entity.h"
#include "ViewModel/GameViewModel/components/buff.h"

class ActiveEntity : public Entity {
public:
Buff buff;
};