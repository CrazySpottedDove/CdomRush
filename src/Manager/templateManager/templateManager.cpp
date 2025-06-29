#include "Manager/templateManager/templateManager.h"
#include "Model/bullets/bullets.h"

TemplateManager::TemplateManager() {
    bullet_map = {
        {BulletType::Arrow, new Arrow()},
        {BulletType::Bolt, new Bolt()},
        {BulletType::Bomb, new Bomb()}
    };
}