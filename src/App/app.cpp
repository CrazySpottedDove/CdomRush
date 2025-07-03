#include "App/app.h"
#include "Common/action.h"
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
        ui_manager.HandleClick();
        switch (game_state) {
        case GameState::Begin:
            store.ClearViewDataQueue();
            if (last_state != GameState::Begin) {
                store.Clear();
                for (const auto& level : *store.resource_manager.GetLevels()) {
                    Fx* fx       = store.template_manager.CreateFx(FxType::LevelFlag);
                    fx->position = level.second;
                    fx->animation.actions.emplace_back(
                        Action(ActionType::SelectLevel, SelectLevelParams{level.first})
                    );
                    store.QueueFx(fx);
                }
                Fx* map_fx = store.template_manager.CreateFx(FxType::Map);

                store.QueueFx(map_fx);
                last_state = GameState::Begin;
                INFO("State Changed to Begin");
            }

            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();

            break;
        case GameState::GameStart:
            store.ClearViewDataQueue();
            if (last_state != GameState::GameStart) {
                store.time = 0;
                store.Clear();
                Fx* fx               = store.template_manager.CreateFx(FxType::Map);
                fx->animation.prefix = store.current_level_name;
                store.QueueFx(fx);
                last_state = GameState::GameStart;
                store.current_subwave_index = 0;
                store.current_wave_index = 0;
                INFO("State Changed to GameStart");
            }
            DEBUG_CODE(
                game_state = GameState::GamePlaying; // For testing purposes, skip loading state
            )
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            store.time += FRAME_LENGTH;
            break;
        case GameState::Loading:
            store.ClearViewDataQueue();

            if (last_state != GameState::Loading) {
                store.Clear();
                // Fx* fx = store.template_manager.CreateFx(FxType::Map);
                // fx->animation.prefix = current_level_name;
                // store.QueueFx(fx);
                store.resource_manager.LoadLevelResources(store.current_level_name);
                store.InitLevel(store.current_level_name);
                last_state = GameState::Loading;
                game_state = GameState::GameStart;
                INFO("State Changed to Loading");
            }
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            break;
        case GameState::GamePlaying:
            store.ClearViewDataQueue();

            if (last_state != GameState::GamePlaying) {
                last_state = GameState::GamePlaying;
                store.current_wave_time = 0;
                INFO("State Changed to GamePlaying");
            }
            window.clear();
            store.SpawnWaves();
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
            store.ClearViewDataQueue();

            if(last_state != GameState::GameOver) {
                store.Clear();
                last_state = GameState::GameOver;
                INFO("State Changed to GameOver");
            }
            window.clear();
            store.UpdateFxs();
            ui_manager.RenderAll();
            window.display();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_LENGTH_IN_MILLISECONDS));

        auto& action_queue = *ui_manager.GetActionQueue();
        while (!action_queue.empty()) {
            Action action = action_queue.front();
            action_queue.pop();
            HandleAction(action);
        }
    }
}

void App::HandleAction(Action& action)
{
    switch (action.type) {
    case ActionType::SelectLevel:
        SelectLevelParams& params = std::get<SelectLevelParams>(action.param);
        store.current_level_name        = params.level_name;
        game_state = GameState::Loading;
    }
}