#include "core/GameEngine.h"
#include "core/InputManager.h"
#include "core/CameraManager.h"
#include "ui/UIManager.h"
#include "systems/LevelManager.h"
#include "systems/AudioManager.h"
#include "physics/PhysicsEngine.h"
#include "utils/Config.h"
#include "utils/Logger.h"

void GameEngine::Initialize() {
    LOG_INFO("Initializing window and subsystems...");

    // Initialize window
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);

    // Initialize audio device
    InitAudioDevice();

    // Initialize subsystems
    inputManager = std::make_unique<InputManager>();
    uiManager = std::make_unique<UIManager>();
    levelManager = std::make_unique<LevelManager>();
    audioManager = std::make_unique<AudioManager>();
    physicsEngine = std::make_unique<PhysicsEngine>();

    // Initialize subsystem dependencies
    levelManager->Initialize();
    audioManager->Initialize();

    // Set up UI callbacks
    uiManager->SetStartGameCallback([this]() {
        SetState(GameState::BIKE_SELECT);
    });

    uiManager->SetQuitCallback([this]() {
        isRunning = false;
    });

    uiManager->SetResumeCallback([this]() {
        SetState(GameState::PLAYING);
        levelManager->ResumeRace();
    });

    // Initial state
    isRunning = true;
    currentState = GameState::MAIN_MENU;
    deltaTime = 0.0f;
    accumulator = 0.0f;

    LOG_INFO("Game engine initialized successfully");
}

void GameEngine::Run() {
    while (isRunning && !WindowShouldClose()) {
        deltaTime = GetFrameTime();

        ProcessInput();
        Update();
        Render();
    }
}

void GameEngine::Update() {
    // Update subsystems based on current state
    switch (currentState) {
        case GameState::MAIN_MENU:
        case GameState::BIKE_SELECT:
        case GameState::LEVEL_SELECT:
        case GameState::PAUSED:
        case GameState::GAME_OVER:
            uiManager->Update(deltaTime);
            break;

        case GameState::PLAYING:
            // Update game systems
            levelManager->Update(deltaTime);
            physicsEngine->Update(deltaTime);
            uiManager->Update(deltaTime);
            
            // Update HUD data for both players
            auto player1 = levelManager->GetPlayer(0);
            auto player2 = levelManager->GetPlayer(1);
            
            if (player1 && player1->GetBike()) {
                uiManager->UpdateHUD(0, 
                    player1->GetBike()->GetCurrentSpeed(),
                    player1->GetCurrentLap(),
                    levelManager->GetCurrentTrack()->GetRequiredLaps(),
                    player1->GetCurrentLapTime(),
                    player1->GetRacePosition());
            }
            
            if (player2 && player2->GetBike()) {
                uiManager->UpdateHUD(1,
                    player2->GetBike()->GetCurrentSpeed(),
                    player2->GetCurrentLap(),
                    levelManager->GetCurrentTrack()->GetRequiredLaps(),
                    player2->GetCurrentLapTime(),
                    player2->GetRacePosition());
            }

            // Check if race is finished
            if (levelManager->IsRaceFinished()) {
                SetState(GameState::GAME_OVER);
            }
            break;
    }

    // Always update audio
    audioManager->Update(deltaTime);
}

void GameEngine::Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Render based on state
    switch (currentState) {
        case GameState::PLAYING:
            // 3D rendering
            levelManager->Render();
            break;

        default:
            // 2D UI only
            break;
    }

    // Always render UI on top
    uiManager->Render();

    // FPS counter (debug)
    DrawFPS(10, 10);

    EndDrawing();
}

void GameEngine::ProcessInput() {
    inputManager->Update();

    // Handle global inputs
    if (currentState == GameState::PLAYING) {
        if (inputManager->IsPausePressed()) {
            SetState(GameState::PAUSED);
            levelManager->PauseRace();
        }
    }

    // Handle menu navigation in UI states
    if (currentState != GameState::PLAYING) {
        // UI handles its own input through InputManager
    }
}

void GameEngine::SetState(GameState newState) {
    LOG_INFO("State transition: " + std::to_string((int)currentState) + " -> " + std::to_string((int)newState));
    
    currentState = newState;

    // Update UI state to match
    switch (newState) {
        case GameState::MAIN_MENU:
            uiManager->SetState(UIState::MAIN_MENU);
            audioManager->PlayMusic("menu");
            break;

        case GameState::BIKE_SELECT:
            uiManager->SetState(UIState::BIKE_SELECT);
            break;

        case GameState::LEVEL_SELECT:
            uiManager->SetState(UIState::LEVEL_SELECT);
            break;

        case GameState::PLAYING:
            uiManager->SetState(UIState::IN_GAME);
            audioManager->PlayMusic("race");
            break;

        case GameState::PAUSED:
            uiManager->SetState(UIState::PAUSE_MENU);
            break;

        case GameState::GAME_OVER:
            uiManager->SetState(UIState::GAME_OVER);
            audioManager->PlayMusic("victory");
            break;
    }
}

void GameEngine::Shutdown() {
    LOG_INFO("Cleaning up resources...");

    // Subsystems will be automatically destroyed via unique_ptr
    
    // Close audio device
    CloseAudioDevice();

    // Close window
    CloseWindow();

    LOG_INFO("Shutdown complete");
}
