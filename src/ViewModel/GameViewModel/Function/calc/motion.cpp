#include "ViewModel/GameViewModel/Function/calc/motion.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/templates/unit.h"
#include <SFML/System/Vector2.hpp>

double calc::real_speed(const Unit& unit)
{
    return (unit.speed + unit.buff.speed_inc) * unit.buff.speed_factor;
}

void calc::enemy_move_tick(const Store& store, Enemy& self)
{
    // const PathManager& path_manager  = store.path_manager;
    PathInfo&       path_info     = self.path_info;
    const Path&     path          = (*(store.resource_manager.GetPaths()))[path_info.path_id];
    const SubPath&  sub_path      = path[path_info.subpath_id];
    const Position& next_waypoint = sub_path[path_info.waypoint_id + 1];
    const double    real_speed    = calc::real_speed(self);

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
                self.heading            = Heading::Right;
                self.animations[0].flip = false;
            }
            else {
                self.heading            = Heading::Up;
                self.animations[0].flip = false;
            }
        }
        else {
            if (displacement.x > -displacement.y) {
                self.heading            = Heading::Right;
                self.animations[0].flip = false;
            }
            else {
                self.heading            = Heading::Down;
                self.animations[0].flip = false;
            }
        }
    }
    else {
        if (displacement.y > 0) {
            if (-displacement.x > displacement.y) {
                self.heading            = Heading::Left;
                self.animations[0].flip = true;
            }
            else {
                self.heading            = Heading::Up;
                self.animations[0].flip = false;
            }
        }
        else {
            if (-displacement.x > -displacement.y) {
                self.heading            = Heading::Left;
                self.animations[0].flip = true;
            }
            else {
                self.heading            = Heading::Down;
                self.animations[0].flip = false;
            }
        }
    }
}

void calc::soldier_move_tick(const Store& store, Soldier& self, Position target_position)
{
    const sf::Vector2f displacement = target_position - self.position;
    if (displacement.lengthSquared() < 0.0001f) {
        INFO("Soldier move tick: already at target position");
        return;
    }
    INFO("Soldier move tick: displacement = " << displacement.x << ", " << displacement.y);
    const sf::Vector2f direction  = displacement.normalized();
    const double       real_speed = calc::real_speed(self);
    const float        movement   = real_speed * FRAME_LENGTH;

    if (displacement.lengthSquared() <= movement * movement) {
        self.position = target_position;
        return;
    }

    self.position += movement * direction;

    if (displacement.x > 0) {
        if (displacement.y > 0) {
            if (displacement.x > displacement.y) {

                self.animations[0].flip = false;
            }
            else {

                self.animations[0].flip = false;
            }
        }
        else {
            if (displacement.x > -displacement.y) {

                self.animations[0].flip = false;
            }
            else {

                self.animations[0].flip = false;
            }
        }
    }
    else {
        if (displacement.y > 0) {
            if (-displacement.x > displacement.y) {
                self.animations[0].flip = true;
            }
            else {
                self.animations[0].flip = false;
            }
        }
        else {
            if (-displacement.x > -displacement.y) {
                self.animations[0].flip = true;
            }
            else {
                self.animations[0].flip = false;
            }
        }
    }
    return;
}

bool calc::enemy_reached_defence_point(const Store& store, const Enemy& self)
{
    const Paths*   paths   = store.resource_manager.GetPaths();
    const Path*    path    = &((*paths)[self.path_info.path_id]);
    const SubPath* subpath = &((*path)[self.path_info.subpath_id]);

    return (self.path_info.waypoint_id + 1 == subpath->size());
}
