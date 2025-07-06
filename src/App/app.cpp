#include "App/app.h"
#include "Common/action.h"
#include "Common/macros.h"
#include "Common/sound.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/fx/fx.h"
#include <SFML/Graphics/Font.hpp>
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
    ui_manager.SetFont(store.resource_manager.GetFont());
    sound_manager.SetSoundDataQueue(store.GetSoundDataQueue());
    sound_manager.SetSoundBufferMap(store.resource_manager.GetSoundBufferMap());
    sound_manager.SetSoundGroupMap(store.resource_manager.GetSoundGroupMap());
    sound_manager.SetMusicSet(store.resource_manager.GetMusicSet());
}

void App::Run()
{
    while (window.isOpen()) {
        switch (game_state) {
        case GameState::Begin:
            if (last_state != GameState::Begin) {
                store.Clear();
                store.QueueSoundData(SoundData(BACKGROUND_SOUND));
                for (const auto& level : *store.resource_manager.GetLevels()) {
                    Fx* fx       = store.template_manager.CreateFx(FxType::LevelFlag);
                    fx->position = level.second;
                    fx->animations[0].actions.emplace_back(
                        Action(ActionType::SelectLevel, SelectLevelParams{level.first}));
                    store.QueueFx(fx);
                }
                Fx* map_fx = store.template_manager.CreateFx(FxType::Map);

                store.QueueFx(map_fx);
                last_state = GameState::Begin;
                INFO("State Changed to Begin");
            }
            DEBUG_CODE(store.current_level_name = "acaroth"; game_state = GameState::Loading;)
            store.UpdateFxs();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
            break;
        case GameState::Loading:
            if (last_state != GameState::Loading) {
                store.Clear();
                store.InitLevel();
                last_state = GameState::Loading;
                game_state = GameState::GameStart;
                INFO("State Changed to Loading");
            }
            store.UpdateFxs();
            ui_manager.PrecessUI();
            break;
        case GameState::GameStart:
            if (last_state != GameState::GameStart) {
                store.gold = 70;
                store.time = 0;
                store.ClearFxs();
                Fx* map_fx                   = store.template_manager.CreateFx(FxType::Map);
                map_fx->animations[0].prefix = store.current_level_name;
                store.QueueFx(map_fx);
                Fx* gold_fx = store.template_manager.CreateFx(FxType::GoldStat);
                store.QueueFx(gold_fx);
                store.QueueSoundData(SoundData(store.current_level_prepare_music));
                last_state                  = GameState::GameStart;
                store.current_subwave_index = 0;
                store.current_wave_index    = 0;
                INFO("State Changed to GameStart");
            }
            DEBUG_CODE(game_state =
                           GameState::GamePlaying;   // For testing purposes, skip loading state
            )
            store.UpdateFxs();
            store.UpdateActionFxs();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
            store.time += FRAME_LENGTH;
            break;

        case GameState::GamePlaying:
            if (last_state != GameState::GamePlaying) {
                last_state              = GameState::GamePlaying;
                store.current_wave_time = 0;
                store.QueueSoundData(SoundData(store.current_level_fight_music));
                INFO("State Changed to GamePlaying");
            }
            store.SpawnWaves();
            store.UpdateDamageEvents();
            store.UpdateFxs();
            store.UpdateEnemies();
            store.UpdateBullets();
            store.UpdateTowers();
            store.UpdateSoldiers();
            store.UpdateActionFxs();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
            store.time += FRAME_LENGTH;
            if (store.life <= 0) {
                game_state = GameState::GameOver;
                INFO("Game Over! Your life is smaller than 0.");
            }
            break;
        case GameState::GameOver:
            if (last_state != GameState::GameOver) {
                store.Clear();
                last_state = GameState::GameOver;
                INFO("State Changed to GameOver");
            }
            store.UpdateFxs();
            ui_manager.PrecessUI();
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
    INFO("Handling action: " << static_cast<int>(action.type));
    switch (action.type) {
    case ActionType::SelectLevel:
    {
        SelectLevelParams& params = std::get<SelectLevelParams>(action.param);
        store.current_level_name  = params.level_name;
        game_state                = GameState::Loading;
        break;
    }
    case ActionType::CreateActionFx:
    {
        INFO("Creating ActionFx with type: " << static_cast<int>(action.type));
        CreateActionFxParams& fx_params = std::get<CreateActionFxParams>(action.param);
        ActionFx*                   action_fx = store.template_manager.CreateActionFx(fx_params.fx_type);
        action_fx->position             = fx_params.position + fx_params.offset;
        action_fx->source_id = fx_params.id;
        std::get<UpgradeTowerParams>(action_fx->animations[0].actions[0].param).tower_id = fx_params.id;
        store.QueueActionFx(action_fx);
        break;
    }
    case ActionType::Delete:{
        store.ClearActionFxs();
        break;
    }
    case ActionType::UpgradeTower:
    {
        UpgradeTowerParams& params = std::get<UpgradeTowerParams>(action.param);
        if(store.gold < params.cost){
            WARNING("Not enough gold to upgrade tower.");
            break;
        }
        store.gold -= params.cost;
        Tower* old_tower = store.GetTower(params.tower_id);
        Tower* new_tower = store.template_manager.CreateTower(params.new_tower_type);
        new_tower->position = old_tower->position;
        new_tower->total_price = old_tower->total_price + params.cost;
        store.DequeueTower(old_tower->id);
        store.QueueTower(new_tower);
        break;
    }
    case ActionType::SellTower:
    {
        SellTowerParams& params = std::get<SellTowerParams>(action.param);
        Tower* tower = store.GetTower(params.tower_id);

        break;
    }
    }
}