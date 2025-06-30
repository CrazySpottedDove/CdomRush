#pragma once
#include <SFML/System/Vector2.hpp>
#define FPS  60
#define IMAGES_PATH "assets/images/"
#define LEVEL_MAP_FILE "data/level_map.lua"
#define LEVEL_DATA_PATH "data/levels/"
#define SOUNDS_PATH "assets/sounds/"
#define PATH_PATH "data/paths/"
#define DEFAULT_SCREEN_WIDTH 1920
#define DEFAULT_SCREEN_LENGTH 1080
constexpr double FRAME_LENGTH = 1.0 / FPS;

#define INVALID_ID -1
typedef int ID;
typedef sf::Vector2f Position;