#pragma once
// filepath: \home\dove\CdomRush\src\utils\readLua.h
#include <iostream>
#include <sol/state.hpp>
#include <sol/table.hpp>
#include <string>

inline sol::table ReadLua(const std::string& file)
{
    static sol::state lua = []() {
        sol::state l;
        l.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string, sol::lib::math);
        return l;
    }();
    try {
        std::cout << 1;
        lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string);
        std::cout << 2;
        const sol::object result = lua.script_file(file);
        std::cout <<3;
        return result.as<sol::table>();
    }
    catch (const sol::error& e) {
        std::cerr << "Error loading Lua file '" << file << "': " << e.what() << std::endl;
        return sol::state{}.create_table();   // 返回空表
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading file '" << file << "': " << e.what() << std::endl;
        return sol::state{}.create_table();   // 返回空表
    }
}