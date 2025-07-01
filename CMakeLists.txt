cmake_minimum_required(VERSION 3.20)
project(CdomRush)

# 显示当前构建类型
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "未设置构建类型，使用默认配置")
endif()

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 根据构建类型添加调试宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    message(STATUS "启用调试模式宏")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
    message(STATUS "启用发布模式宏")
endif()

# 查找SFML包 - 需要指定组件
find_package(SFML COMPONENTS System Network Graphics Audio Window CONFIG REQUIRED)

find_package(lua REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# 添加可执行文件
add_executable(CdomRush
    ${SOURCES}
)

# 链接SFML库 - 使用新的目标名称
target_link_libraries(CdomRush PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
    ${LUA_LIBRARIES}
)

# 设置包含目录
target_include_directories(CdomRush PRIVATE src ${LUA_INCLUDE_DIR})

# Linux特定设置
if(UNIX AND NOT APPLE)
    # 如果需要静态链接
    # set_target_properties(CdomRush PROPERTIES LINK_SEARCH_START_STATIC 1)
    # set_target_properties(CdomRush PROPERTIES LINK_SEARCH_END_STATIC 1)

    # 添加pthread支持（Linux上通常需要）
    find_package(Threads REQUIRED)
    target_link_libraries(CdomRush PRIVATE Threads::Threads)
endif()