cmake_minimum_required(VERSION 3.20)
project(CdomRush)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找SFML包 - 需要指定组件
find_package(SFML COMPONENTS System Network Graphics Audio Window CONFIG REQUIRED)

# 添加可执行文件
add_executable(CdomRush
    src/main.cpp
    src/store/store.cpp
    # 添加其他源文件...
)

# 链接SFML库 - 使用新的目标名称
target_link_libraries(CdomRush PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)

# 设置包含目录
target_include_directories(CdomRush PRIVATE src)

# Linux特定设置
if(UNIX AND NOT APPLE)
    # 如果需要静态链接
    # set_target_properties(CdomRush PROPERTIES LINK_SEARCH_START_STATIC 1)
    # set_target_properties(CdomRush PROPERTIES LINK_SEARCH_END_STATIC 1)

    # 添加pthread支持（Linux上通常需要）
    find_package(Threads REQUIRED)
    target_link_libraries(CdomRush PRIVATE Threads::Threads)
endif()