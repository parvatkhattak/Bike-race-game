#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "raylib.h"
#include <string>
#include <functional>

class Player; // Forward declaration

enum class UIState {
    MAIN_MENU,
    BIKE_SELECT,
    LEVEL_SELECT,
    IN_GAME,
    PAUSE_MENU,
    GAME_OVER
};

class UIManager {
public:
    UIManager();
    ~UIManager() = default;

    void Update(float deltaTime);
    void Render() const;
    void SetState(UIState state);
    UIState GetState() const { return currentState; }

    // Menu callbacks
    void SetStartGameCallback(std::function<void()> callback) { onStartGame = callback; }
    void SetQuitCallback(std::function<void()> callback) { onQuit = callback; }
    void SetResumeCallback(std::function<void()> callback) { onResume = callback; }

    // In-game HUD data
    void UpdateHUD(int playerID, float speed, int currentLap, int totalLaps, float lapTime, int position);

private:
    // Rendering
    void RenderMainMenu() const;
    void RenderBikeSelect() const;
    void RenderLevelSelect() const;
    void RenderInGameHUD(const Player& player1, const Player& player2) const;
    void RenderPauseMenu() const;
    void RenderGameOver() const;

    // Helper rendering functions
    void DrawButton(Rectangle bounds, const std::string& text, Color color, bool highlighted) const;
    void DrawTitle(const std::string& title, int yPos) const;
    void DrawPlayerHUD(int playerID, Rectangle hudArea) const;

    UIState currentState;
    int selectedMenuOption;
    int maxMenuOptions;
    int selectedBikeIndex; // 0 = Red bike, 1 = Blue bike

    // HUD data for each player
    struct HUDData {
        float speed;
        int currentLap;
        int totalLaps;
        float lapTime;
        int position;
    };
    
    HUDData player1HUD;
    HUDData player2HUD;

    // Callbacks
    std::function<void()> onStartGame;
    std::function<void()> onQuit;
    std::function<void()> onResume;

    // Visual settings
    Font titleFont;
    bool fontLoaded;
};

#endif // UIMANAGER_H
