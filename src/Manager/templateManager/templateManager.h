#pragma once

#include "Model/bullets/bullets.h"
#include <unordered_map>
class TemplateManager{
    public:
        std::unordered_map<BulletType, Bullet*> bullet_map;
    TemplateManager();
};