#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#define FPS 30
#define IMAGES_PATH "assets/images/"
#define LEVEL_MAP_FILE "data/level_map.lua"
#define LEVEL_DATA_PATH "data/levels/"
#define SOUNDS_PATH "assets/sounds/"
#define LEVEL_SPECIFIC_SOUNDS_PATH "assets/sounds/levelSpecific/"
#define PATH_PATH "data/paths/"
#define WAVE_PATH "data/waves/"
#define TOWER_PATH "data/towers/"
#define DEFAULT_SCREEN_WIDTH 1920
#define DEFAULT_SCREEN_HEIGHT 1080
#define DEFAULT_WINDOW_TITLE "CdomRush"
#define ORIGIN_SCREEN_WIDTH 1024
#define ORIGIN_SCREEN_HEIGHT 768
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_BOLD "\x1b[1m"
#define ANSI_COLOR_DIM "\x1b[2m"
#define WIDTH_FACTOR 1.428571f
#define HEIGHT_FACTOR 1.40625f
#define BACKGROUND_SOUND "MusicMap1"
#define DEFAULT_FONT_FILE "assets/fonts/msyh.ttc"
#define MAP_LAYER 0
#define COMMON_LAYER 1
#define FX_LAYER 2
#define HEALTH_BAR_LAYER 3
#define UI_LOWER_LAYER 4
#define UI_MIDDLE_LAYER 5
#define UI_UPPER_LAYER 6
#define TEXT_LAYER 7
#define GAMEOVER_BACKGROUND_LAYER 8
#define GAMEOVER_TEXT_LAYER 9
#define X_CENTER 500
#define Y_CENTER 384

#ifdef DEBUG
#    include <iostream>
#    define INFO(message)                                                                \
        std::cerr << ANSI_COLOR_CYAN << ANSI_COLOR_BOLD << "[INFO] " << ANSI_COLOR_RESET \
                  << ANSI_COLOR_CYAN << message << ANSI_COLOR_RESET << std::endl

#    define ERROR(message)                                                               \
        std::cerr << ANSI_COLOR_RED << ANSI_COLOR_BOLD << "[ERROR] " << ANSI_COLOR_RESET \
                  << ANSI_COLOR_RED << message << ANSI_COLOR_RESET << std::endl

#    define SUCCESS(message)                                                                 \
        std::cout << ANSI_COLOR_GREEN << ANSI_COLOR_BOLD << "[SUCCESS] " << ANSI_COLOR_RESET \
                  << ANSI_COLOR_GREEN << message << ANSI_COLOR_RESET << std::endl

#    define WARNING(message)                                                                  \
        std::cerr << ANSI_COLOR_YELLOW << ANSI_COLOR_BOLD << "[WARNING] " << ANSI_COLOR_RESET \
                  << ANSI_COLOR_YELLOW << message << ANSI_COLOR_RESET << std::endl
#    define DEBUG_CODE(debug_code) {debug_code}
#else
#    define DEBUG_CODE(debug_code) \
        {}
#    define INFO(message) \
        {}
#    define ERROR(message) \
        {}
#    define SUCCESS(message) \
        {}
#    define WARNING(message) \
        {}
#endif

constexpr double FRAME_LENGTH                 = 1.0 / FPS;
constexpr int    FRAME_LENGTH_IN_MILLISECONDS = FRAME_LENGTH * 1000;
#define INVALID_ID -1
typedef int                   ID;
typedef sf::Vector2f          Position;
typedef std::vector<Position> SubPath;
typedef std::vector<SubPath>  Path;
typedef std::vector<Path>     Paths;
