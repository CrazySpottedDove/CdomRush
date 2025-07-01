#include "Manager/templateManager/templateManager.h"
#include "Model/bullets/bullets.h"

TemplateManager::TemplateManager() {
    bullet_map = {
        // {BulletType::Arrow, new Arrow()},
        // {BulletType::Bolt, new Bolt()},
        // {BulletType::Bomb, new Bomb()}
    };
    tower_map = {

    };
    enemy_map = {

    };
}

Bullet* TemplateManager::CreateBullet(const BulletType type) const {
    auto it = bullet_map.find(type);
#
    return it->second->Clone();
}