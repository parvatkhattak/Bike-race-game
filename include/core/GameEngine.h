#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "raylib.h"
#include <memory>

// Forward declarations
class InputManager;
class UIManager;
class LevelManager;
class AudioManager;
class PhysicsEngine;

enum class GameState {
    MAIN_MENU,
    BIKE_SELECT,
    LEVEL_SELECT,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameEngine {
public:
    static GameEngine& GetInstance() {
        static GameEngine instance;
        return instance;
    }

    // Core functions
    void Initialize();
    void Run();
    void Shutdown();

    // State management
    void SetState(GameState newState);
    GameState GetState() const { return currentState; }

    // Delta time
    float GetDeltaTime() const { return deltaTime; }

    // Subsystem access
    InputManager* GetInputManager() const { return inputManager.get(); }
    UIManager* GetUIManager() const { return uiManager.get(); }
    LevelManager* GetLevelManager() const { return levelManager.get(); }
    AudioManager* GetAudioManager() const { return audioManager.get(); }
    PhysicsEngine* GetPhysicsEngine() const { return physicsEngine.get(); }

private:
    GameEngine() = default;
    ~GameEngine() = default;
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    void Update();
    void Render();
    void ProcessInput();

    // State
    bool isRunning;
    GameState currentState;
    float deltaTime;
    float accumulator;

    // Subsystems
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<UIManager> uiManager;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<PhysicsEngine> physicsEngine;
};

#endif // GAMEENGINE_H
