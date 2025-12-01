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
    trackData.requiredLaps = 1; // Changed to 1 lap - just reach the finish line
    trackData.player1SpawnPoint = {0, 0.5f, -80}; // Start far back
    trackData.player2SpawnPoint = {0, 0.5f, -80};
    trackData.spawnDirection = {0, 0, 1}; // Face forward
    
    // Create straight-line checkpoints leading to finish
    checkpoints.clear();
    
    // Checkpoints every 20 units in a straight line forward
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, -60}, 15.0f, 0));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, -40}, 15.0f, 1));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, -20}, 15.0f, 2));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 0}, 15.0f, 3));
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 20}, 15.0f, 4)); // Intermediate
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 40}, 15.0f, 5)); // Intermediate
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 60}, 15.0f, 6)); // Near finish
    checkpoints.push_back(std::make_unique<Checkpoint>(Vector3{0, 0.5f, 80}, 20.0f, 7)); // FINISH LINE
    
    // Add solid obstacles for challenge
    obstacles.clear();
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{15, 1, -30}, ObstacleType::STATIC_BARRIER, Vector3{3, 3, 3}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-15, 1, -10}, ObstacleType::STATIC_BARRIER, Vector3{3, 3, 3}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{12, 1, 10}, ObstacleType::STATIC_BARRIER, Vector3{3, 3, 3}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-12, 1, 30}, ObstacleType::STATIC_BARRIER, Vector3{3, 3, 3}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{18, 1, 55}, ObstacleType::STATIC_BARRIER, Vector3{4, 4, 4}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-18, 1, 55}, ObstacleType::STATIC_BARRIER, Vector3{4, 4, 4}));
    
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
    
    // Solid obstacles
    obstacles.clear();
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{12, 1, 8}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{-12, 1, -8}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    obstacles.push_back(std::make_unique<Obstacle>(Vector3{18, 1, 0}, ObstacleType::MOVING_PLATFORM, Vector3{3, 1, 3}));
    
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
    
    // Many solid obstacles for challenge
    obstacles.clear();
    for (int i = 0; i < 8; i++) {
        float angle = (i / 8.0f) * 2.0f * PI;
        float x = cosf(angle) * 20.0f;
        float z = sinf(angle) * 20.0f;
        obstacles.push_back(std::make_unique<Obstacle>(Vector3{x, 1, z}, ObstacleType::STATIC_BARRIER, Vector3{2, 2, 2}));
    }
    
    LOG_INFO("Advanced track created");
}

void Track::LoadTrackModel() {
    // Create a larger ground plane for the track (200x200 instead of 100x100)
    Mesh planeMesh = GenMeshPlane(200.0f, 200.0f, 10, 10);
    trackModel = LoadModelFromMesh(planeMesh);
    trackModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
    modelLoaded = true;
    
    trackBounds.min = {-100, 0, -100};
    trackBounds.max = {100, 5, 100};
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
    
    // Draw MASSIVE finish line at the last checkpoint (now the actual finish)
    if (!checkpoints.empty()) {
        Vector3 finishPos = checkpoints.back()->GetPosition(); // Last checkpoint is finish
        
        // Draw giant red and white checkered finish line
        for (int i = -10; i <= 10; i++) {
            Color stripColor = (i % 2 == 0) ? RED : WHITE;
            DrawCube({finishPos.x + i * 3.0f, 0.2f, finishPos.z}, 3.0f, 0.4f, 10.0f, stripColor);
        }
        
        // Draw tall finish line pillars
        DrawCylinder({finishPos.x - 30.0f, 15.0f, finishPos.z}, 2.0f, 2.0f, 30.0f, 16, Fade(RED, 0.8f));
        DrawCylinder({finishPos.x + 30.0f, 15.0f, finishPos.z}, 2.0f, 2.0f, 30.0f, 16, Fade(RED, 0.8f));
        
        // Giant "FINISH" banner
        DrawCube({finishPos.x, 30.0f, finishPos.z}, 70.0f, 3.0f, 2.0f, GOLD);
        DrawCube({finishPos.x, 32.0f, finishPos.z}, 65.0f, 2.0f, 1.5f, RED);
        
        // Also draw start line at first checkpoint
        Vector3 startPos = checkpoints[0]->GetPosition();
        for (int i = -8; i <= 8; i++) {
            Color stripColor = (i % 2 == 0) ? GREEN : WHITE;
            DrawCube({startPos.x + i * 2.5f, 0.1f, startPos.z}, 2.5f, 0.2f, 5.0f, stripColor);
        }
        DrawCube({startPos.x, 8.0f, startPos.z}, 40.0f, 2.0f, 1.0f, LIME);
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
