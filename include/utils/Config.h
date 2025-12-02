#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    // Window Settings
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;
    constexpr int TARGET_FPS = 60;
    constexpr const char* WINDOW_TITLE = "Bike Race Game";

    // Game Settings
    constexpr int MAX_PLAYERS = 2;
    constexpr int DEFAULT_LAPS = 3;
    constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

    // Physics Constants
    constexpr float GRAVITY = 9.8f;
    constexpr float GROUND_FRICTION = 0.92f;
    constexpr float AIR_DRAG = 0.98f;

    // Bike Default Stats
    constexpr float BIKE_BASE_SPEED = 50.0f;
    constexpr float BIKE_BASE_ACCELERATION = 22.0f;  // Increased for more responsive acceleration
    constexpr float BIKE_BASE_TURN_RATE = 150.0f;     // Balanced for responsive yet smooth turning
    constexpr float BIKE_BASE_BRAKE_FORCE = 32.0f;    // Increased for stronger braking power

    // Camera Settings
    constexpr float CAMERA_DISTANCE = 15.0f;
    constexpr float CAMERA_HEIGHT = 8.0f;
    constexpr float CAMERA_SMOOTHNESS = 0.1f;

    // File Paths
    const std::string ASSETS_PATH = "assets/";
    const std::string MODELS_PATH = ASSETS_PATH + "models/";
    const std::string TEXTURES_PATH = ASSETS_PATH + "textures/";
    const std::string AUDIO_PATH = ASSETS_PATH + "audio/";
    const std::string DATA_PATH = ASSETS_PATH + "data/";
    const std::string SAVE_FILE = "playerdata.json";

    // Colors
    constexpr unsigned char COLOR_PRIMARY_R = 0;
    constexpr unsigned char COLOR_PRIMARY_G = 121;
    constexpr unsigned char COLOR_PRIMARY_B = 241;
    constexpr unsigned char COLOR_PRIMARY_A = 255;

    constexpr unsigned char COLOR_SECONDARY_R = 255;
    constexpr unsigned char COLOR_SECONDARY_G = 69;
    constexpr unsigned char COLOR_SECONDARY_B = 0;
    constexpr unsigned char COLOR_SECONDARY_A = 255;
}

#endif // CONFIG_H
