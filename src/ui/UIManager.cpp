#include "ui/UIManager.h"
#include "core/GameEngine.h"
#include "core/InputManager.h"
#include "systems/LevelManager.h"
#include "utils/Config.h"
#include <string>

UIManager::UIManager() :
    currentState(UIState::MAIN_MENU),
    selectedMenuOption(0),
    maxMenuOptions(3),
    selectedBikeIndex(0), // Default to red bike
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
        
        // Bike selection uses up/down to choose bike
        if (currentState == UIState::BIKE_SELECT) {
            if (input.IsMenuDownPressed()) {
                selectedBikeIndex = (selectedBikeIndex + 1) % 2; // Toggle between 0 and 1
            }
            if (input.IsMenuUpPressed()) {
                selectedBikeIndex = (selectedBikeIndex - 1 + 2) % 2; // Toggle backwards
            }
        } else {
            // Other menus use up/down for option selection
            if (input.IsMenuDownPressed()) {
                selectedMenuOption = (selectedMenuOption + 1) % maxMenuOptions;
            }
            if (input.IsMenuUpPressed()) {
                selectedMenuOption = (selectedMenuOption - 1 + maxMenuOptions) % maxMenuOptions;
            }
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
                    // Bike selected, go to level select
                    SetState(UIState::LEVEL_SELECT);
                    break;
                    
                case UIState::LEVEL_SELECT: {
                    // Start game with selected bike
                    auto& levelMgr = *GameEngine::GetInstance().GetLevelManager();
                    levelMgr.LoadLevel(selectedMenuOption + 1, selectedBikeIndex);
                    levelMgr.StartRace();
                    GameEngine::GetInstance().SetState(GameState::PLAYING);
                    break;
                }
                    
                case UIState::GAME_OVER:
                    // Return to main menu and reset
                    SetState(UIState::MAIN_MENU);
                    GameEngine::GetInstance().SetState(GameState::MAIN_MENU);
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
        case UIState::IN_GAME: {
            auto& levelMgr = *GameEngine::GetInstance().GetLevelManager();
            if (levelMgr.GetPlayer(0) && levelMgr.GetPlayer(1)) {
                RenderInGameHUD(*levelMgr.GetPlayer(0), *levelMgr.GetPlayer(1));
            }
            break;
        }
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
    // Background gradient effect
    DrawRectangleGradientV(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, 
                           (Color){20, 30, 50, 255}, (Color){40, 60, 90, 255});
    
    // Title with shadow
    const char* title = "BIKE RACE GAME";
    int titleWidth = MeasureText(title, 80);
    int titleX = (Config::SCREEN_WIDTH - titleWidth) / 2;
    DrawText(title, titleX + 3, 103, 80, (Color){0, 0, 0, 180}); // Shadow
    DrawText(title, titleX, 100, 80, ORANGE);
    
    // Subtitle
    const char* subtitle = "Two-Player Racing Action";
    int subWidth = MeasureText(subtitle, 25);
    DrawText(subtitle, (Config::SCREEN_WIDTH - subWidth) / 2, 200, 25, SKYBLUE);
    
    // Menu options with better styling
    const char* menuOptions[] = {"START GAME", "OPTIONS", "QUIT"};
    for (int i = 0; i < 3; i++) {
        Color optionColor = (i == selectedMenuOption) ? YELLOW : WHITE;
        Color bgColor = (i == selectedMenuOption) ? (Color){255, 200, 0, 100} : (Color){70, 80, 100, 150};
        
        int textWidth = MeasureText(menuOptions[i], 35);
        int boxX = (Config::SCREEN_WIDTH - textWidth - 60) / 2;
        int boxY = 300 + i * 70;
        
        // Draw selection box
        DrawRectangle(boxX, boxY - 10, textWidth + 60, 50, bgColor);
        DrawRectangleLines(boxX, boxY - 10, textWidth + 60, 50, optionColor);
        
        // Draw text
        DrawText(menuOptions[i], boxX + 30, boxY, 35, optionColor);
    }
    
    // Controls hint with icons
    DrawRectangle(200, 550, 880, 120, (Color){0, 0, 0, 150});
    DrawText("CONTROLS", 520, 565, 30, GOLD);
    DrawText("Player 1: W/A/S/D + LEFT SHIFT (Nitro)", 250, 605, 22, LIGHTGRAY);
    DrawText("Player 2: ARROW KEYS + RIGHT SHIFT (Nitro)", 250, 635, 22, LIGHTGRAY);
    
    DrawText("Press ENTER to continue", 440, 680, 20, YELLOW);
}

void UIManager::RenderBikeSelect() const {
    DrawTitle("SELECT YOUR BIKE", 100);
    
    const char* bikeOptions[] = {"RED BIKE", "BLUE BIKE"};
    Color bikeColors[] = {RED, BLUE};
    
    for (int i = 0; i < 2; i++) {
        bool isSelected = (i == selectedBikeIndex);
        Color textColor = isSelected ? YELLOW : bikeColors[i];
        Color bgColor = isSelected ? ColorAlpha(bikeColors[i], 0.3f) : ColorAlpha(bikeColors[i], 0.1f);
        int fontSize = isSelected ? 40 : 32;
        
        int textWidth = MeasureText(bikeOptions[i], fontSize);
        int boxX = (Config::SCREEN_WIDTH - textWidth - 80) / 2;
        int boxY = 300 + i * 100;
        
        // Draw selection box
        DrawRectangle(boxX, boxY - 10, textWidth + 80, 60, bgColor);
        if (isSelected) {
            DrawRectangleLines(boxX, boxY - 10, textWidth + 80, 60, YELLOW);
            DrawText(">", boxX - 40, boxY, fontSize, YELLOW);
        }
        
        // Draw bike name
        DrawText(bikeOptions[i], boxX + 40, boxY, fontSize, textColor);
    }
    
    DrawText("Use ARROW KEYS to select", Config::SCREEN_WIDTH / 2 - 150, 520, 22, LIGHTGRAY);
    DrawText("Press ENTER to continue", Config::SCREEN_WIDTH / 2 - 135, Config::SCREEN_HEIGHT - 50, 20, YELLOW);
}

void UIManager::RenderLevelSelect() const {
    // Dark gradient background for better contrast
    DrawRectangleGradientV(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, 
                           (Color){20, 30, 50, 255}, (Color){40, 60, 90, 255});
    
    DrawTitle("SELECT TRACK", 100);
    
    const char* tracks[] = {"Beginner Circuit", "Intermediate Track", "Advanced Track"};
    const char* difficulty[] = {"[EASY]", "MEDIUM]", "[HARD]"};
    
    for (int i = 0; i < 3; i++) {
        bool isSelected = (i == selectedMenuOption);
        Color color = isSelected ? GOLD : LIGHTGRAY; // Gold for selected, lightgray for others
        int fontSize = isSelected ? 38 : 30;
        
        std::string text = std::string(tracks[i]) + " " + difficulty[i];
        int textWidth = MeasureText(text.c_str(), fontSize);
        int x = Config::SCREEN_WIDTH / 2 - textWidth / 2;
        int y = 300 + i * 80;
        
        // Draw selection box if selected
        if (isSelected) {
            DrawRectangle(x - 20, y - 10, textWidth + 40, fontSize + 20, ColorAlpha(GOLD, 0.2f));
            DrawRectangleLines(x - 20, y - 10, textWidth + 40, fontSize + 20, GOLD);
        }
        
        DrawText(text.c_str(), x, y, fontSize, color);
    }
    
    DrawText("Press ENTER to start race", Config::SCREEN_WIDTH / 2 - 135, Config::SCREEN_HEIGHT - 50, 22, YELLOW);
}

void UIManager::RenderInGameHUD(const Player& player1, const Player& player2) const {
    // Player 1 HUD (left side) - Improved design
    DrawRectangle(10, 10, 280, 180, (Color){0, 0, 0, 180});
    DrawRectangleLines(10, 10, 280, 180, RED);
    
    DrawText("PLAYER 1", 25, 20, 28, RED);
    DrawRectangle(25, 52, 100, 3, RED);
    
    std::string speed1 = std::to_string((int)player1.GetBike()->GetSpeed());
    DrawText("SPEED", 25, 65, 18, LIGHTGRAY);
    DrawText((speed1 + " km/h").c_str(), 25, 85, 24, WHITE);
    
    std::string lap1 = std::to_string(player1.GetCurrentLap()) + "/3";
    DrawText("LAP", 25, 115, 18, LIGHTGRAY);
    DrawText(lap1.c_str(), 25, 135, 24, GOLD);
    
    std::string pos1 = player1.GetRacePosition() == 1 ? "1st" : "2nd";
    DrawText("POS", 160, 65, 18, LIGHTGRAY);
    DrawText(pos1.c_str(), 160, 85, 32, player1.GetRacePosition() == 1 ? GREEN : ORANGE);
    
    std::string time1 = std::to_string((int)player1.GetCurrentLapTime()) + "s";
    DrawText("TIME", 160, 130, 18, LIGHTGRAY);
    DrawText(time1.c_str(), 160, 150, 20, SKYBLUE);
    
    // Player 2 HUD (right side) - Improved design
    int p2X = Config::SCREEN_WIDTH - 290;
    DrawRectangle(p2X, 10, 280, 180, (Color){0, 0, 0, 180});
    DrawRectangleLines(p2X, 10, 280, 180, BLUE);
    
    DrawText("CPU", p2X + 15, 20, 28, BLUE);
    DrawRectangle(p2X + 15, 52, 100, 3, BLUE);
    
    std::string speed2 = std::to_string((int)player2.GetBike()->GetSpeed());
    DrawText("SPEED", p2X + 15, 65, 18, LIGHTGRAY);
    DrawText((speed2 + " km/h").c_str(), p2X + 15, 85, 24, WHITE);
    
    std::string lap2 = std::to_string(player2.GetCurrentLap()) + "/3";
    DrawText("LAP", p2X + 15, 115, 18, LIGHTGRAY);
    DrawText(lap2.c_str(), p2X + 15, 135, 24, GOLD);
    
    std::string pos2 = player2.GetRacePosition() == 1 ? "1st" : "2nd";
    DrawText("POS", p2X + 150, 65, 18, LIGHTGRAY);
    DrawText(pos2.c_str(), p2X + 150, 85, 32, player2.GetRacePosition() == 1 ? GREEN : ORANGE);
    
    std::string time2 = std::to_string((int)player2.GetCurrentLapTime()) + "s";
    DrawText("TIME", p2X + 150, 130, 18, LIGHTGRAY);
    DrawText(time2.c_str(), p2X + 150, 150, 20, SKYBLUE);
    
    // Center instruction
    const char* hint = "ESC - Pause";
    DrawText(hint, (Config::SCREEN_WIDTH - MeasureText(hint, 18)) / 2, 10, 18, (Color){255, 255, 255, 150});
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
    DrawRectangleGradientV(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, 
                           (Color){10, 10, 20, 255}, (Color){30, 20, 40, 255});
    
    auto& levelMgr = *GameEngine::GetInstance().GetLevelManager();
    int winner = levelMgr.GetWinner();
    Player* player1 = levelMgr.GetPlayer(0);
    Player* player2 = levelMgr.GetPlayer(1);
    
    if (!player1 || !player2) return;
    
    // Confetti animation (simple particles)
    for (int i = 0; i < 50; i++) {
        int x = ((int)(GetTime() * 50) + i * 37) % Config::SCREEN_WIDTH;
        int y = ((int)(GetTime() * 80) + i * 23) % Config::SCREEN_HEIGHT;
        Color confettiColor = (i % 4 == 0) ? GOLD : (i % 4 == 1) ? RED : (i % 4 == 2) ? BLUE : GREEN;
        DrawCircle(x, y, 5, confettiColor);
    }
    
    // Victory Title with shadow
    const char* title = "🏁 RACE COMPLETE! 🏁";
    int titleWidth = MeasureText(title, 70);
    DrawText(title, (Config::SCREEN_WIDTH - titleWidth) / 2 + 3, 53, 70, (Color){0, 0, 0, 180});
    DrawText(title, (Config::SCREEN_WIDTH - titleWidth) / 2, 50, 70, GOLD);
    
    // Winner announcement
    Player* winnerPlayer = (winner == 0) ? player1 : player2;
    std::string winnerText = (winner == 0) ? "PLAYER 1 WINS!" : "CPU WINS!";
    Color winnerColor = (winner == 0) ? RED : BLUE;
    
    int winnerWidth = MeasureText(winnerText.c_str(), 60);
    DrawRectangle(Config::SCREEN_WIDTH / 2 - winnerWidth / 2 - 40, 150, winnerWidth + 80, 100, (Color){0, 0, 0, 200});
    DrawRectangleLines(Config::SCREEN_WIDTH / 2 - winnerWidth / 2 - 40, 150, winnerWidth + 80, 100, winnerColor);
    DrawText(winnerText.c_str(), Config::SCREEN_WIDTH / 2 - winnerWidth / 2, 170, 60, winnerColor);
    
    // Crown emoji for winner
    DrawText("👑", Config::SCREEN_WIDTH / 2 - 30, 120, 60, GOLD);
    
    // Podium/Results Box
    int boxY = 280;
    DrawRectangle(240, boxY, 800, 280, (Color){0, 0, 0, 180});
    DrawRectangleLines(240, boxY, 800, 280, GOLD);
    
    DrawText("RACE RESULTS", 450, boxY + 15, 35, YELLOW);
    DrawRectangle(450, boxY + 55, 200, 3, YELLOW);
    
    // Player 1 stats
    int p1Y = boxY + 75;
    std::string p1Pos = (player1->GetRacePosition() == 1) ? "1st" : "2nd";
    Color p1PosColor = (player1->GetRacePosition() == 1) ? GOLD : LIGHTGRAY;
    
    DrawText("PLAYER 1", 270, p1Y, 28, RED);
    DrawText(p1Pos.c_str(), 450, p1Y, 28, p1PosColor);
    
    std::string p1Laps = "Laps: " + std::to_string(player1->GetCurrentLap());
    DrawText(p1Laps.c_str(), 550, p1Y, 22, LIGHTGRAY);
    
    std::string p1Time = "Best Lap: " + std::to_string((int)player1->GetBestLapTime()) + "s";
    DrawText(p1Time.c_str(), 720, p1Y, 22, SKYBLUE);
    
    std::string p1Points = "+" + std::to_string((player1->GetRacePosition() == 1) ? 100 : 50) + " pts";
    DrawText(p1Points.c_str(), 270, p1Y + 35, 20, GREEN);
    
    // Player 2 stats
    int p2Y = boxY + 155;
    std::string p2Pos = (player2->GetRacePosition() == 1) ? "1st" : "2nd";
    Color p2PosColor = (player2->GetRacePosition() == 1) ? GOLD : LIGHTGRAY;
    
    DrawText("CPU", 270, p2Y, 28, BLUE);
    DrawText(p2Pos.c_str(), 450, p2Y, 28, p2PosColor);
    
    std::string p2Laps = "Laps: " + std::to_string(player2->GetCurrentLap());
    DrawText(p2Laps.c_str(), 550, p2Y, 22, LIGHTGRAY);
    
    std::string p2Time = "Best Lap: " + std::to_string((int)player2->GetBestLapTime()) + "s";
    DrawText(p2Time.c_str(), 720, p2Y, 22, SKYBLUE);
    
    std::string p2Points = "+" + std::to_string((player2->GetRacePosition() == 1) ? 100 : 50) + " pts";
    DrawText(p2Points.c_str(), 270, p2Y + 35, 20, GREEN);
    
    // Total points display
    DrawRectangle(350, boxY + 220, 500, 45, (Color){30, 30, 30, 200});
    std::string totalP1 = "P1 Total: " + std::to_string(player1->GetTotalPoints()) + " pts";
    std::string totalP2 = "P2 Total: " + std::to_string(player2->GetTotalPoints()) + " pts";
    DrawText(totalP1.c_str(), 380, boxY + 230, 24, RED);
    DrawText(totalP2.c_str(), 620, boxY + 230, 24, BLUE);
    
    // Continue prompt
    const char* prompt = "Press ENTER to return to menu";
    DrawRectangle(380, 590, 520, 50, (Color){0, 0, 0, 150});
    DrawText(prompt, 400, 600, 25, YELLOW);
    
    // Fun fact for winner
    if (GetTime() - (int)GetTime() < 0.5) {  // Blink effect
        const char* celebration = (winner == 0) ? "🎉 PLAYER 1 IS CHAMPION! 🎉" : "🎉 CPU IS CHAMPION! 🎉";
        int celebWidth = MeasureText(celebration, 20);
        DrawText(celebration, (Config::SCREEN_WIDTH - celebWidth) / 2, 660, 20, GOLD);
    }
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
