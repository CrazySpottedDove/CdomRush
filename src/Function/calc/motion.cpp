#include "Function/calc/motion.h"
#include "Manager/pathManager/pathManager.h"
#include "Manager/store/store.h"
#include "Model/enemies/enemies.h"
#include "Model/templates/unit.h"
#include "utils/macros.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>

double calc::real_speed(const Unit& unit)
{
    return (unit.speed + unit.buff.speed_inc) * unit.buff.speed_factor;
}

void calc::enemy_move_tick(const Store& store, Enemy& self)
{
    const PathManager& path_manager  = store.path_manager;
    PathInfo&          path_info     = self.path_info;
    const SubPath&     sub_path      = path_manager.GetSubPath(path_info);
    const Position&    next_waypoint = sub_path[path_info.waypoint_id + 1];
    const double       real_speed    = calc::real_speed(self);

    const sf::Vector2f displacement = next_waypoint - self.position;
    const sf::Vector2f direction    = displacement.normalized();

    const float movement = real_speed * FRAME_LENGTH;
    self.position += direction * movement;

    if (displacement.lengthSquared() < movement * movement) {
        ++path_info.waypoint_id;
    }

    // 更新敌人的面向
    if (displacement.x > 0) {
        if (displacement.y > 0) {
            if (displacement.x > displacement.y) {
                self.heading = Heading::Right;
                self.animation.flip = false;
            }
            else {
                self.heading = Heading::Down;
                self.animation.flip = false;
            }
        }
        else {
            if( displacement.x > -displacement.y) {
                self.heading = Heading::Right;
                self.animation.flip = false;
            }
            else {
                self.heading = Heading::Up;
                self.animation.flip = false;
            }
        }
    }else {
        if (displacement.y > 0) {
            if (-displacement.x > displacement.y) {
                self.heading = Heading::Left;
                self.animation.flip = true;
            }
            else {
                self.heading = Heading::Down;
                self.animation.flip = false;
            }
        }
        else {
            if (-displacement.x > -displacement.y) {
                self.heading = Heading::Left;
                self.animation.flip = true;
            }
            else {
                self.heading = Heading::Up;
                self.animation.flip = false;
            }
        }
    }
    DEBUG_CODE(
        std::cerr << "Enemy moved to " << self.position.x << ", " << self.position.y
                  << " with heading " << static_cast<int>(self.heading) << std::endl;
        std::cerr << "Next waypoint: " << next_waypoint.x << ", " << next_waypoint.y
                  << std::endl;
    )
}

bool calc::enemy_reached_defence_point(const Store& store, const Enemy& self)
{
    return store.path_manager.IsPathEnd(self.path_info);
}

void calc::map_position(Position& pos){
    const float default_screen_width = 1920.0;
    const float default_screen_height = 1080.0;
    const float origin_screen_width = 1024.0 * 8.4 / 6.4;
    const float origin_screen_height = 768.0;
    pos.x = default_screen_width * pos.x / origin_screen_width;
    pos.y = default_screen_height - (default_screen_height * pos.y / origin_screen_height);
    pos.x += 225.0f;
    pos.y -= 35.0f;
    // pos.y = DEFAULT_SCREEN_HEIGHT - pos.y; // Adjust y-coordinate to match screen origin
    // pos.x += 150.0f;
    // pos.y -= 30.0f;
}