#include "core/GameEngine.h"
#include "utils/Logger.h"
#include "utils/Config.h"

int main() {
    // Initialize logger
    Logger::GetInstance().Init("game.log");
    LOG_INFO("=== Bike Race Game Starting ===");

    // Get game engine instance
    GameEngine& engine = GameEngine::GetInstance();

    // Initialize engine
    LOG_INFO("Initializing game engine...");
    engine.Initialize();

    // Run game loop
    LOG_INFO("Starting game loop...");
    engine.Run();

    // Shutdown
    LOG_INFO("Shutting down game engine...");
    engine.Shutdown();

    LOG_INFO("=== Bike Race Game Terminated ===");
    return 0;
}
