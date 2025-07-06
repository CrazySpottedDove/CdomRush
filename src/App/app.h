#pragma once

#include "Common/action.h"
#include "View/soundManager.h"
#include "View/UIManager.h"
#include "ViewModel/GameViewModel/store/store.h"
#include <SFML/Graphics/RenderWindow.hpp>

enum class GameState
{
    Begin,         // 游戏打开时的界面，用于选择关卡
    Loading,       // 加载资源时显示的等待界面
    GameStart,     // 开始一场游戏，等待玩家点击开始按钮
    GamePlaying,   // 游戏进行中
    GameOver,      // 游戏结束，显示游戏结果
    None, // 未定义状态
};

class App{
public:
    App();
    void Run();
private:
    void HandleAction(Action& action);
    Store store;
    UIManager ui_manager;
    SoundManager sound_manager;
    GameState last_state = GameState::None;
    GameState game_state = GameState::Begin;
    sf::RenderWindow window;
};