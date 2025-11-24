#include "ui/UIManager.h"
#include "core/GameEngine.h"
#include "utils/Config.h"
#include <string>

UIManager::UIManager() :
    currentState(UIState::MAIN_MENU),
    selectedMenuOption(0),
    maxMenuOptions(3),
    fontLoaded(false)
{
    // Initialize HUD data
    player1HUD = {0.0f, 1, 3, 0.0f, 1};
    player2HUD = {0.0f, 1, 3, 0.0f, 2};
}

void UIManager::Update(float deltaTime) {
    // Handle menu navigation
    if (currentState != UIState::IN_GAME) {
        auto& input = *GameEngine::GetInstance().GetInputManager();
        
        if (input.IsMenuDownPressed()) {
            selectedMenuOption = (selectedMenuOption + 1) % maxMenuOptions;
        }
        if (input.IsMenuUpPressed()) {
            selectedMenuOption = (selectedMenuOption - 1 + maxMenuOptions) % maxMenuOptions;
        }
        
        // Handle selection
        if (input.IsConfirmPressed()) {
            switch (currentState) {
                case UIState::MAIN_MENU:
                    if (selectedMenuOption == 0 && onStartGame) onStartGame();
                    else if (selectedMenuOption == 1) { /* Options */ }
                    else if (selectedMenuOption == 2 && onQuit) onQuit();
                    break;
                    
                case UIState::PAUSE_MENU:
                    if (selectedMenuOption == 0 && onResume) onResume();
                    else if (selectedMenuOption == 1) { /* Restart */ }
                    else if (selectedMenuOption == 2) SetState(UIState::MAIN_MENU);
                    break;
                    
                case UIState::BIKE_SELECT:
                    // Start level select
                    SetState(UIState::LEVEL_SELECT);
                    break;
                    
                case UIState::LEVEL_SELECT:
                    // Start game
                    auto& levelMgr = *GameEngine::GetInstance().GetLevelManager();
                    levelMgr.LoadLevel(selectedMenuOption + 1);
                    levelMgr.StartRace();
                    GameEngine::GetInstance().SetState(GameState::PLAYING);
                    break;
            }
        }
    }
}

void UIManager::Render() const {
    switch (currentState) {
        case UIState::MAIN_MENU:
            RenderMainMenu();
            break;
        case UIState::BIKE_SELECT:
            RenderBikeSelect();
            break;
        case UIState::LEVEL_SELECT:
            RenderLevelSelect();
            break;
        case UIState::IN_GAME:
            RenderInGameHUD();
            break;
        case UIState::PAUSE_MENU:
            RenderPauseMenu();
            break;
        case UIState::GAME_OVER:
            RenderGameOver();
            break;
    }
}

void UIManager::SetState(UIState state) {
    currentState = state;
    selectedMenuOption = 0;
    
    // Set max menu options based on state
    switch (state) {
        case UIState::MAIN_MENU: maxMenuOptions = 3; break;
        case UIState::PAUSE_MENU: maxMenuOptions = 3; break;
        case UIState::LEVEL_SELECT: maxMenuOptions = 3; break;
        default: maxMenuOptions = 1; break;
    }
}

void UIManager::UpdateHUD(int playerID, float speed, int currentLap, int totalLaps, float lapTime, int position) {
    HUDData& hud = (playerID == 0) ? player1HUD : player2HUD;
    hud.speed = speed;
    hud.currentLap = currentLap;
    hud.totalLaps = totalLaps;
    hud.lapTime = lapTime;
    hud.position = position;
}

void UIManager::RenderMainMenu() const {
    const int screenW = Config::SCREEN_WIDTH;
    const int screenH = Config::SCREEN_HEIGHT;
    
    DrawTitle("BIKE RACE GAME", 100);
    
    const char* menuItems[] = {"START GAME", "OPTIONS", "QUIT"};
    int startY = screenH / 2;
    
    for (int i = 0; i < 3; i++) {
        Color color = (i == selectedMenuOption) ? YELLOW : WHITE;
        int fontSize = (i == selectedMenuOption) ? 40 : 30;
        const char* text = menuItems[i];
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, screenW / 2 - textWidth / 2, startY + i * 60, fontSize, color);
    }
    
    DrawText("Press ENTER to select", screenW / 2 - 100, screenH - 50, 20, LIGHTGRAY);
}

void UIManager::RenderBikeSelect() const {
    DrawTitle("SELECT YOUR BIKE", 100);
    
    DrawText("Player 1: Red Bike", 200, 300, 30, RED);
    DrawText("Player 2: Blue Bike", 200, 350, 30, BLUE);
    
    DrawText("Press ENTER to continue", Config::SCREEN_WIDTH / 2 - 120, Config::SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
}

void UIManager::RenderLevelSelect() const {
    DrawTitle("SELECT TRACK", 100);
    
    const char* tracks[] = {"Beginner Circuit", "Intermediate Track", "Advanced Track"};
    const char* difficulty[] = {"[EASY]", "[MEDIUM]", "[HARD]"};
    
    for (int i = 0; i < 3; i++) {
        Color color = (i == selectedMenuOption) ? YELLOW : WHITE;
        int fontSize = (i == selectedMenuOption) ? 35 : 28;
        
        std::string text = std::string(tracks[i]) + " " + difficulty[i];
        int textWidth = MeasureText(text.c_str(), fontSize);
        DrawText(text.c_str(), Config::SCREEN_WIDTH / 2 - textWidth / 2, 300 + i * 70, fontSize, color);
    }
    
    DrawText("Press ENTER to start race", Config::SCREEN_WIDTH / 2 - 120, Config::SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
}

void UIManager::RenderInGameHUD() const {
    // Split screen HUDs
    Rectangle player1Area = {0, 0, (float)Config::SCREEN_WIDTH / 2, (float)Config::SCREEN_HEIGHT};
    Rectangle player2Area = {(float)Config::SCREEN_WIDTH / 2, 0, (float)Config::SCREEN_WIDTH / 2, (float)Config::SCREEN_HEIGHT};
    
    DrawPlayerHUD(0, player1Area);
    DrawPlayerHUD(1, player2Area);
    
    // Center divider
    DrawRectangle(Config::SCREEN_WIDTH / 2 - 2, 0, 4, Config::SCREEN_HEIGHT, WHITE);
}

void UIManager::RenderPauseMenu() const {
    // Semi-transparent overlay
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));
    
    DrawTitle("PAUSED", 150);
    
    const char* menuItems[] = {"RESUME", "RESTART", "MAIN MENU"};
    int startY = Config::SCREEN_HEIGHT / 2;
    
    for (int i = 0; i < 3; i++) {
        Color color = (i == selectedMenuOption) ? YELLOW : WHITE;
        int fontSize = (i == selectedMenuOption) ? 35 : 28;
        const char* text = menuItems[i];
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, Config::SCREEN_WIDTH / 2 - textWidth / 2, startY + i * 60, fontSize, color);
    }
}

void UIManager::RenderGameOver() const {
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, ColorAlpha(BLACK, 0.8f));
    
    DrawTitle("RACE FINISHED!", 150);
    
    DrawText("Press ENTER to continue", Config::SCREEN_WIDTH / 2 - 120, Config::SCREEN_HEIGHT - 50, 20, YELLOW);
}

void UIManager::DrawPlayerHUD(int playerID, Rectangle hudArea) const {
    const HUDData& hud = (playerID == 0) ? player1HUD : player2HUD;
    Color playerColor = (playerID == 0) ? RED : BLUE;
    
    int x = (int)hudArea.x + 20;
    int y = (int)hudArea.y + 20;
    
    // Player name
    std::string playerName = "PLAYER " + std::to_string(playerID + 1);
    DrawText(playerName.c_str(), x, y, 25, playerColor);
    
    // Speed
    std::string speedText = "SPEED: " + std::to_string((int)hud.speed) + " km/h";
    DrawText(speedText.c_str(), x, y + 40, 20, WHITE);
    
    // Lap
    std::string lapText = "LAP: " + std::to_string(hud.currentLap) + "/" + std::to_string(hud.totalLaps);
    DrawText(lapText.c_str(), x, y + 70, 20, WHITE);
    
    // Lap time
    char timeStr[32];
    sprintf(timeStr, "TIME: %.2f s", hud.lapTime);
    DrawText(timeStr, x, y + 100, 20, WHITE);
    
    // Position
    std::string posText = "POSITION: " + std::to_string(hud.position);
    Color posColor = (hud.position == 1) ? GOLD : LIGHTGRAY;
    DrawText(posText.c_str(), x, y + 130, 20, posColor);
}

void UIManager::DrawTitle(const std::string& title, int yPos) const {
    int fontSize = 60;
    int textWidth = MeasureText(title.c_str(), fontSize);
    DrawText(title.c_str(), Config::SCREEN_WIDTH / 2 - textWidth / 2, yPos, fontSize, GOLD);
}

void UIManager::DrawButton(Rectangle bounds, const std::string& text, Color color, bool highlighted) const {
    Color bgColor = highlighted ? ColorAlpha(color, 0.5f) : ColorAlpha(color, 0.2f);
    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLinesEx(bounds, 2, color);
    
    int textWidth = MeasureText(text.c_str(), 24);
    DrawText(text.c_str(), 
             (int)bounds.x + ((int)bounds.width - textWidth) / 2,
             (int)bounds.y + ((int)bounds.height - 24) / 2,
             24, WHITE);
}
