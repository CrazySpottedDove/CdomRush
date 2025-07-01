#include "Manager/pathManager/pathManager.h"
#include "Function/calc/motion.h"
#include "utils/macros.h"
#include "utils/readLua.h"
#include <iostream>
#include <sol/state.hpp>
#include <sol/types.hpp>

void PathManager::ReadPathsFromLua(const std::string& file)
{
    try {
        const sol::table paths_table = ReadLua(file);

        // 遍历每个路径
        for (const auto& path_pair : paths_table) {

            const sol::table path_table = path_pair.second.as<sol::table>();

            Path current_path;

            // 遍历路径中的每个子路径
            for (const auto& subpath_pair : path_table) {
                const sol::table subpath_table = subpath_pair.second.as<sol::table>();

                SubPath current_subpath;

                // 遍历子路径中的每个路径点
                for (const auto& waypoint_pair : subpath_table) {
                    const sol::table waypoint_table = waypoint_pair.second.as<sol::table>();
                    // 解析Position
                    Position waypoint(waypoint_table["x"].get<double>(),
                                      waypoint_table["y"].get<double>());
                    calc::map_position(waypoint);
                    current_subpath.push_back(waypoint);
                }

                current_path.push_back(current_subpath);
            }

            paths.push_back(current_path);
        }

        std::cout << "Successfully loaded " << paths.size() << " paths" << std::endl;
    }
    catch (const sol::error& e) {
        std::cerr << "Lua error reading paths from '" << file << "': " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading paths from '" << file << "': " << e.what() << std::endl;
    }
}