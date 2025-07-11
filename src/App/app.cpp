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
#include <cstddef>
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

App::~App()
{
}

void App::Run()
{
    while (window.isOpen()) {
        switch (game_state) {
        case GameState::Begin:
            if (last_state != GameState::Begin) {
                store.IntoBegin();
                last_state = GameState::Begin;
                INFO("State Changed to Begin");
            }
            // DEBUG_CODE(store.current_level_name = "acaroth"; game_state = GameState::Loading;)
            store.UpdateFxs();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
            break;
        case GameState::Loading:
            if (last_state != GameState::Loading) {
                store.IntoLoading();
                last_state = GameState::Loading;
                game_state = GameState::GameStart;
                INFO("State Changed to Loading");
            }
            store.UpdateFxs();
            ui_manager.PrecessUI();
            break;
        case GameState::GameStart:
            if (last_state != GameState::GameStart) {
                last_state = GameState::GameStart;
                store.IntoGameStart();
                INFO("State Changed to GameStart");
            }
            // DEBUG_CODE(game_state =
            //                GameState::GamePlaying;   // For testing purposes, skip loading state
            // )
            store.UpdateFxs();
            store.UpdateActionFxs();
            store.UpdateTowers();
            store.UpdateSoldiers();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
            store.time += FRAME_LENGTH;
            break;

        case GameState::GamePlaying:
            if (last_state != GameState::GamePlaying) {
                last_state              = GameState::GamePlaying;
                store.IntoGamePlaying();
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
            if (store.life <= 0 || (store.current_wave_index >= store.resource_manager.GetWaves()->size() &&
                                    store.GetEnemies().empty())) {
                game_state = GameState::GameOver;
            }
            break;
        case GameState::GameOver:
            if (last_state != GameState::GameOver) {
                store.IntoGameOver();
                last_state = GameState::GameOver;
                INFO("State Changed to GameOver");
            }
            store.UpdateFxs();
            store.UpdateTowers();
            store.UpdateSoldiers();
            store.UpdateActionFxs();
            ui_manager.PrecessUI();
            sound_manager.PlayAll();
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
        const FxType          fx_type   = fx_params.fx_type;
        switch (fx_type) {
        case FxType::CommonUpgradeButton:
        {
            ActionFx* action_fx =
                new CommonUpgradeButton(std::get<UpgradeTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::UpgradeToArcherButton:
        {
            ActionFx* action_fx =
                new UpgradeToArcherButton(std::get<UpgradeTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::UpgradeToMageButton:
        {
            ActionFx* action_fx =
                new UpgradeToMageButton(std::get<UpgradeTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }

        case FxType::UpgradeToEngineerButton:
        {
            ActionFx* action_fx =
                new UpgradeToEngineerButton(std::get<UpgradeTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::UpgradeToBarrackButton:
        {
            ActionFx* action_fx =
                new UpgradeToBarrackButton(std::get<UpgradeTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::SellTowerButton:
        {
            ActionFx* action_fx =
                new SellTowerButton(std::get<SellTowerParams>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::PlainActionText:
        {
            ActionFx* action_fx = new PlainActionTextFx(std::get<std::string>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::TowerRange:
        {
            ActionFx* action_fx = new TowerRangeFx(std::get<ID>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            INFO("Creating TowerRangeFx at position: (" +
                 std::to_string(action_fx->position.x) + ", " +
                 std::to_string(action_fx->position.y) + ")");
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::ChangeRallyPointButton:
        {
            ActionFx* action_fx =
                new BarrackChangeRallyPointButton(std::get<ID>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        case FxType::BarrackRange:
        {
            ActionFx* action_fx = new BarrackRangeFx(std::get<ID>(fx_params.props));
            action_fx->position = fx_params.position + fx_params.offset;
            store.QueueActionFx(action_fx);
            break;
        }
        }
        break;
    }
    case ActionType::Delete:
    {
        store.ClearActionFxs();
        break;
    }
    case ActionType::UpgradeTower:
    {
        UpgradeTowerParams& params = std::get<UpgradeTowerParams>(action.param);
        store.QueueSoundData(SoundData(params.sound_name));
        if (store.gold < params.cost) {
            WARNING("Not enough gold to upgrade tower.");
            break;
        }
        store.gold -= params.cost;
        Tower* old_tower       = store.GetTower(params.tower_id);
        Tower* new_tower       = store.template_manager.CreateTower(params.new_tower_type);
        new_tower->position    = old_tower->position;
        new_tower->total_price = old_tower->total_price + params.cost;
        new_tower->rally_point = old_tower->rally_point;
        store.DequeueTower(old_tower->id);
        store.QueueTower(new_tower);
        break;
    }
    case ActionType::SellTower:
    {
        store.QueueSoundData(SoundData("GUICoins"));
        SellTowerParams& params = std::get<SellTowerParams>(action.param);
        Tower*           tower  = store.GetTower(params.tower_id);
        if(tower){
            store.gold += tower->total_price * 0.5;
            Tower* new_build_terrain    = store.template_manager.CreateTower(TowerType::None);
            new_build_terrain->position = tower->position;
            new_build_terrain->rally_point = tower->rally_point;
            store.DequeueTower(tower->id);
            store.QueueTower(new_build_terrain);
            INFO("Tower sold for " << tower->total_price * 0.5 << " gold.");
        }
        break;
    }
    case ActionType::BackToBegin:
    {
        game_state = GameState::Begin;
        break;
    }
    case ActionType::EndGameStart:
    {
        if(game_state == GameState::GameStart){
            game_state = GameState::GamePlaying;
        }
        break;
    }
    case ActionType::ChangeRallyPoint:{
        ChangeRallyPointParams& params = std::get<ChangeRallyPointParams>(action.param);
        Tower*                  tower  = store.GetTower(params.tower_id);
        if (tower) {
            tower->rally_point = params.new_rally_point;
            INFO("Changed rally point to: (" + std::to_string(tower->rally_point.x) + ", " +
                 std::to_string(tower->rally_point.y) + ")");
        } else {
            WARNING("Tower with ID " + std::to_string(params.tower_id) + " not found.");
        }
        break;
    }
    }
}
