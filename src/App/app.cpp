#include "App/app.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/fx/fx.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <chrono>
#include <thread>

App::App()
    : window(
          sf::RenderWindow(sf::VideoMode(sf::Vector2u(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)),
                           DEFAULT_WINDOW_TITLE, sf::Style::Default))
{
    ui_manager.SetWindow(window);
    ui_manager.SetTextureMap(*store.resource_manager.GetTextureMap());
    ui_manager.SetSpriteFrameDataMap(*store.resource_manager.GetSpriteFrameDataMap());
    ui_manager.SetAnimationGroupMap(*store.resource_manager.GetAnimationGroupMap());
    ui_manager.SetViewData(*store.GetViewDataQueue());
}

void App::Run()
{
    while (window.isOpen()) {
        switch (game_state) {
        case GameState::Begin:
            if (last_state != GameState::Begin) {
                store.Clear();
                for (const auto& level : *store.resource_manager.GetLevels()) {
                    Fx* fx       = store.template_manager.CreateFx(FxType::LevelFlag);
                    fx->position = level.second;
                    store.QueueFx(fx);
                }
                Fx* map_fx = store.template_manager.CreateFx(FxType::Map);
                store.QueueFx(map_fx);
                last_state = GameState::Begin;
            }

            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            break;
        case GameState::GameStart:
            if (last_state != GameState::GameStart) {
                store.time = 0;
                store.Clear();
                Fx* fx               = store.template_manager.CreateFx(FxType::Map);
                fx->animation.prefix = current_level_name;
                store.QueueFx(fx);
                last_state = GameState::GameStart;
            }
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            store.time += FRAME_LENGTH;
            break;
        case GameState::Loading:
            if (last_state != GameState::Loading) {
                store.Clear();
                // Fx* fx = store.template_manager.CreateFx(FxType::Map);
                // fx->animation.prefix = current_level_name;
                // store.QueueFx(fx);
                last_state = GameState::Loading;
            }
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            break;
        case GameState::GamePlaying:
            if (last_state != GameState::GamePlaying) {
                last_state = GameState::GamePlaying;
            }
            window.clear();
            store.UpdateDamageEvents();
            store.UpdateFxs();
            store.UpdateEnemies();
            store.UpdateBullets();
            store.UpdateTowers();
            store.UpdateSoldiers();
            ui_manager.RenderAll();
            window.display();
            store.time += FRAME_LENGTH;
            if(store.life <= 0){
                game_state = GameState::GameOver;
                INFO("Game Over! Your life is smaller than 0.");
            }
            break;
        case GameState::GameOver:
            if(last_state != GameState::GameOver) {
                store.Clear();
                last_state = GameState::GameOver;
            }
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_LENGTH_IN_MILLISECONDS));
    }
}