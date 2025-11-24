#include "level/Track.h"
#include "utils/Logger.h"
#include <cmath>

Track::Track() : modelLoaded(false) {
    trackData.name = "Unnamed Track";
    trackData.difficulty = 1;
    trackData.requiredLaps = 3;
    trackData.player1SpawnPoint = {-5, 0.5f, 0};
    trackData.player2SpawnPoint = {5, 0.5f, 0};
    trackData.spawnDirection = {0, 0, 1};
}

bool Track::LoadTrack(const std::string& trackName) {
    LOG_INFO("Loading track: " + trackName);
    
    if (trackName == "Beginner Circuit" || trackName == "1") {
        CreateBeginnerTrack();
    } else if (trackName == "Intermediate Track" || trackName == "2") {
        CreateIntermediateTrack();
    } else if (trackName == "Advanced Track" || trackName == "3") {
        CreateAdvancedTrack();
    } else {
        CreateBeginnerTrack(); // Default
    }
    
    LoadTrackModel();
    return true;
}

void Track::CreateBeginnerTrack() {
    trackData.name = "Beginner Circuit";
    trackData.difficulty = 1;
    trackData.requiredLaps = 3;
    trackData.player1SpawnPoint = {-3, 0.5f, -20};
    trackData.player2SpawnPoint = {3, 0.5f, -20};
    trackData.spawnDirection = {0, 0, 1};
    
    // Create simple oval track checkpoints
    checkpoints.clear();
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 20}, 8.0f, 0));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{20, 0.5f, 20}, 8.0f, 1));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{20, 0.5f, -20}, 8.0f, 2));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-20, 0.5f, -20}, 8.0f, 3));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-20, 0.5f, 20}, 8.0f, 4));
    
    // Add a few simple obstacles
    obstacles.clear();
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{10, 1, 0}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-10, 1, 10}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    
    LOG_INFO("Beginner track created with " + std::to_string(checkpoints.size()) + " checkpoints");
}

void Track::CreateIntermediateTrack() {
    trackData.name = "Intermediate Track";
    trackData.difficulty = 2;
    trackData.requiredLaps = 3;
    trackData.player1SpawnPoint = {-3, 0.5f, -25};
    trackData.player2SpawnPoint = {3, 0.5f, -25};
    trackData.spawnDirection = {0, 0, 1};
    
    // More complex track layout
    checkpoints.clear();
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 25}, 7.0f, 0));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{25, 0.5f, 15}, 7.0f, 1));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{25, 0.5f, -15}, 7.0f, 2));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, -25}, 7.0f, 3));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-25, 0.5f, -15}, 7.0f, 4));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-25, 0.5f, 15}, 7.0f, 5));
    
    // More obstacles
    obstacles.clear();
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{12, 1, 8}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-12, 1, -8}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{18, 1, 0}, ObstacleType::MOVING_PLATFORM, Vector3{3, 1, 3}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{0, 0.1f, 0}, ObstacleType::SPEED_BOOST, Vector3{5, 0.1f, 5}));
    
    LOG_INFO("Intermediate track created");
}

void Track::CreateAdvancedTrack() {
    trackData.name = "Advanced Track";
    trackData.difficulty = 3;
    trackData.requiredLaps = 3;
    trackData. player1SpawnPoint = {-3, 0.5f, -30};
    trackData.player2SpawnPoint = {3, 0.5f, -30};
    trackData.spawnDirection = {0, 0, 1};
    
    // Complex figure-8 style track
    checkpoints.clear();
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 30}, 6.0f, 0));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{30, 0.5f, 20}, 6.0f, 1));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{30, 0.5f, 0}, 6.0f, 2));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, -10}, 6.0f, 3));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-30, 0.5f, 0}, 6.0f, 4));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{-30, 0.5f, 20}, 6.0f, 5));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 10}, 6.0f, 6));
    
    // Many obstacles for challenge
    obstacles.clear();
    for (int i = 0; i < 8; i++) {
        float angle = (i / 8.0f) * 2.0f * PI;
        float x = cosf(angle) * 20.0f;
        float z = sinf(angle) * 20.0f;
        obstacles.push_back(std::make_unique<Obstacle>(Vector3{x, 1, z}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    }
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{15, 0.1f, 10}, ObstacleType::SLOW_ZONE, Vector3{6, 0.1f, 6}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-15, 0.1f, -10}, ObstacleType::SPEED_BOOST, Vector3{5, 0.1f, 5}));
    
    LOG_INFO("Advanced track created");
}

void Track::LoadTrackModel() {
    // Create a simple ground plane for the track
    Mesh planeMesh = GenMeshPlane(100.0f, 100.0f, 1, 1);
    trackModel = LoadModelFromMesh(planeMesh);
    trackModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = ColorAlpha(DARKGREEN, 0.8f);
    modelLoaded = true;
    
    trackBounds.min = {-50, 0, -50};
    trackBounds.max = {50, 5, 50};
}

void Track::Update(float deltaTime) {
    // Update obstacles
    for (auto& obstacle : obstacles) {
        obstacle->Update(deltaTime);
    }
}

void Track::Render() const {
    if (modelLoaded) {
        DrawModel(trackModel, {0, 0, 0}, 1.0f, WHITE);
    }
    
    // Render obstacles
    for (const auto& obstacle : obstacles) {
        obstacle->Render();
    }
}

void Track::RenderDebug() const {
    // Render checkpoints
    for (const auto& checkpoint : checkpoints) {
        checkpoint->Render();
    }
    
    // Render track bounds
    DrawBoundingBox(trackBounds, BLUE);
}

bool Track::CheckCheckpoint(int playerID, Vector3 bikePosition, int currentCheckpoint) {
    if (currentCheckpoint < 0 || currentCheckpoint >= (int)checkpoints.size()) {
        return false;
    }
    
    const float BIKE_RADIUS = 2.0f;
    return checkpoints[currentCheckpoint]->CheckPassage(bikePosition, BIKE_RADIUS);
}

Vector3 Track::GetSpawnPoint(int playerID) const {
    return (playerID == 0) ? trackData.player1SpawnPoint : trackData.player2SpawnPoint;
}
