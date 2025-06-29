#include "Function/calc/motion.h"
#include "Manager/pathManager/pathManager.h"
#include "Manager/store/store.h"
#include "Model/enemies/enemies.h"
#include "Model/templates/unit.h"
#include "utils/macros.h"
#include <SFML/System/Vector2.hpp>




void calc::enemy_move_tick(const Store& store, Enemy& self)
{
    const PathManager& path_manager  = store.path_manager;
    PathInfo&    path_info     = self.path_info;
    const SubPath&     sub_path      = path_manager.GetSubPath(path_info);
    const Position&    next_waypoint = sub_path[path_info.waypoint_id + 1];
    const double real_speed = calc::real_speed(self);

    const sf::Vector2f displacement = next_waypoint - self.position;
    const sf::Vector2f direction = displacement.normalized();

    const float movement = real_speed * FRAME_LENGTH;
    self.position += direction * movement;

    if (displacement.lengthSquared() < movement * movement) {
        ++path_info.waypoint_id;
    }
}