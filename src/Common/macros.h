#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#define FPS 30
#define IMAGES_PATH "assets/images/"
#define LEVEL_MAP_FILE "data/level_map.lua"
#define LEVEL_DATA_PATH "data/levels/"
#define SOUND_PATH "assets/sounds/"
#define PATH_PATH "data/paths/"
#define WAVE_PATH "data/waves/"
#define TOWER_PATH "data/towers/"
#define DEFAULT_SCREEN_WIDTH 1920
#define DEFAULT_SCREEN_HEIGHT 1080
#define DEFAULT_WINDOW_TITLE "CdomRush"
#define ORIGIN_SCREEN_WIDTH 1024
#define ORIGIN_SCREEN_HEIGHT 768
#ifdef DEBUG
#    include <iostream>
#    define DEBUG_CODE(debug_code) {debug_code}
#else
#    define DEBUG_CODE(debug_code) \
        {}
#endif

constexpr double FRAME_LENGTH = 1.0 / FPS;

#define INVALID_ID -1
typedef int          ID;
typedef sf::Vector2f Position;
typedef std::vector<Position> SubPath;
typedef std::vector<SubPath>  Path;
typedef std::vector<Path>     Paths;
