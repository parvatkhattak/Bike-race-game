#include "systems/LevelManager.h"
#include "core/GameEngine.h"
#include "core/InputManager.h"
#include "core/CameraManager.h"
#include "physics/PhysicsEngine.h"
#include "utils/Logger.h"
#include "raymath.h"
#include <algorithm>
#include <random>

LevelManager::LevelManager() :
    raceState(RaceState::NOT_STARTED),
    countdownTimer(3.0f),
    raceTime(0.0f),
    currentLevelID(1)
{
    // Initialize with level 1 unlocked
    unlockedLevels.resize(5, false);
    unlockedLevels[0] = true; // Level 1 unlocked by default
}

void LevelManager::Initialize() {
    LOG_INFO("LevelManager initialized");
    
    // Create 5 players - 1 human + 4 AI opponents
    AddPlayer(0, "Player 1");
    AddPlayer(1, "CPU 1");
    AddPlayer(2, "CPU 2");
    AddPlayer(3, "CPU 3");
    AddPlayer(4, "CPU 4");
    
    // Set all except Player 1 as AI
    for (size_t i = 1; i < players.size(); i++) {
        players[i]->SetAI(true);
    }
}

void LevelManager::LoadLevel(int levelID, int playerBikeIndex) {
    currentLevelID = levelID;
    
    // Create new track
    currentTrack = std::make_unique<Track>();
    
    // Load appropriate track based on level ID
    std::string trackName = "track" + std::to_string(levelID);
    currentTrack->LoadTrack(trackName);
    
    // Reset players with randomized starting grid positions
    Color bikeColors[] = {RED, BLUE, GREEN, YELLOW, ORANGE};
    
    // Create shuffled grid positions for randomization
    std::vector<int> gridPositions = {0, 1, 2, 3, 4};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(gridPositions.begin(), gridPositions.end(), g);
    
    // Swap bike colors if needed so selected bike is always at index 0 (player 0)
    // This ensures player 0 (arrow keys) controls the selected bike
    if (playerBikeIndex == 1) {
        // User selected blue bike, swap red and blue
        std::swap(bikeColors[0], bikeColors[1]);
    }
    
    for (size_t i = 0; i < players.size(); i++) {
        // Use randomized grid position instead of player index
        int gridSlot = gridPositions[i];
        float xOffset = (gridSlot - 2.0f) * 4.0f; // -8, -4, 0, 4, 8
        Vector3 baseSpawn = currentTrack->GetSpawnPoint(0); // Get track start
        Vector3 startPos = {baseSpawn.x + xOffset, baseSpawn.y, baseSpawn.z};
        
        Color bikeColor = bikeColors[i % 5];
        
        // Initialize player with bike
        players[i]->Initialize(startPos, bikeColor);
        
        // Reset player race stats
        players[i]->ResetRace();
    }
    
    // Player 0 is always human (uses arrow keys), all others are AI
    players[0]->SetAI(false);
    for (size_t i = 1; i < players.size(); i++) {
        players[i]->SetAI(true);
    }
    
    std::string bikeChoice = (playerBikeIndex == 0) ? "RED" : "BLUE";
    LOG_INFO("Loaded level " + std::to_string(levelID) + " - Player chose " + bikeChoice + " bike");
}

void LevelManager::Update(float deltaTime) {
    if (!currentTrack) return;
    
    currentTrack->Update(deltaTime);
    
    switch (raceState) {
        case RaceState::COUNTDOWN:
            countdownTimer -= deltaTime;
            if (countdownTimer <= 0.0f) {
                raceState = RaceState::RACING;
                LOG_INFO("Race started!");
            }
            break;
            
        case RaceState::RACING: {
            UpdateRaceProgress(deltaTime);
            CheckCheckpoints();
            CheckCollisions();
            UpdatePlayerPositions();
            
            // Update players
            for (auto& player : players) {
                if (player->IsAI()) {
                    // AI Logic
                    // Get next checkpoint position
                    int nextCPIndex = player->GetCheckpointsPassed();
                    // If nextCPIndex is valid
                    Vector3 targetPos = {0,0,0};
                    if (currentTrack) {
                         // We need a way to get checkpoint position. 
                         // Track::GetCheckpoints() returns vector of unique_ptr<Checkpoint>
                         // Let's assume we can access it or add a helper.
                         // For now, let's look at Track.h/cpp to see if we can get checkpoint pos.
                         // Track has GetCheckpoints().
                         const auto& checkpoints = currentTrack->GetCheckpoints();
                         if (nextCPIndex < (int)checkpoints.size()) {
                             targetPos = checkpoints[nextCPIndex]->GetPosition();
                         } else {
                             // Lap complete, target first checkpoint
                             if (!checkpoints.empty()) targetPos = checkpoints[0]->GetPosition();
                         }
                    }
                    player->UpdateAI(deltaTime, targetPos, currentLevelID); // Pass level as difficulty
                } else {
                    // Human Input
                    auto inputMgr = GameEngine::GetInstance().GetInputManager();
                    float accel = inputMgr->GetAxisValue(player->GetID(), InputAction::ACCELERATE);
                    float brake = inputMgr->GetAxisValue(player->GetID(), InputAction::BRAKE);
                    float turn = inputMgr->GetAxisValue(player->GetID(), InputAction::TURN_RIGHT);
                    bool nitro = inputMgr->IsActionPressed(player->GetID(), InputAction::NITRO);
                    
                    player->ProcessInput(accel, brake, turn, nitro);
                }
                
                player->Update(deltaTime);
                
                // Apply physics
                auto physicsEngine = GameEngine::GetInstance().GetPhysicsEngine();
                physicsEngine->ApplyPhysics(player->GetBike(), deltaTime);
            }
            
            // TODO: Update camera for both players (camera manager recreation causing issues)
            // auto cameraMgr = std::make_unique<CameraManager>();
            // for (auto& player : players) {
            //     Vector3 pos = player->GetBike()->GetPosition();
            //     Vector3 dir = player->GetBike()->GetDirection();
            //     cameraMgr->SetTarget(player->GetID(), pos, dir);
            // }
            break;
        }
            
        case RaceState::FINISHED:
            // Race finished
            break;
            
        default:
            break;
    }
}

void LevelManager::Render() const {
    if (!currentTrack) return;
    
    // Create camera that follows Player 1
    Camera3D camera = { 0 };
    
    if (!players.empty() && players[0]->GetBike()) {
        Vector3 bikePos = players[0]->GetBike()->GetPosition();
        Vector3 bikeDir = players[0]->GetBike()->GetDirection();
        
        // Camera position: behind and above the bike
        Vector3 cameraOffset = {-bikeDir.x * 20.0f, 15.0f, -bikeDir.z * 20.0f};
        camera.position = Vector3Add(bikePos, cameraOffset);
        
        // Look at a point ahead of the bike
        Vector3 targetOffset = {bikeDir.x * 5.0f, 0.0f, bikeDir.z * 5.0f};
        camera.target = Vector3Add(bikePos, targetOffset);
    } else {
        // Fallback camera position
        camera.position = {0, 15, -20};
        camera.target = {0, 0, 0};
    }
    
    camera.up = {0, 1, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    BeginMode3D(camera);
    
    // Render track
    currentTrack->Render();
    
    // Render debug checkpoints
    #ifdef DEBUG
    currentTrack->RenderDebug();
    #endif
    
    // Render players
    for (const auto& player : players) {
        player->Render();
    }
    
    // Draw ground grid for reference
    DrawGrid(50, 2.0f);
    
    EndMode3D();
}

void LevelManager::StartRace() {
    raceState = RaceState::COUNTDOWN;
    countdownTimer = 3.0f;
    raceTime = 0.0f;
    
    for (auto& player : players) {
        player->StartRace();
    }
    
    LOG_INFO("Race countdown started");
}

void LevelManager::EndRace() {
    raceState = RaceState::FINISHED;
    
    // Determine winner and award points
    int winner = GetWinner();
    if (winner >= 0 && winner < (int)players.size()) {
        // Award points based on position
        for (auto& player : players) {
            if (player->GetRacePosition() == 1) {
                player->AddPoints(100); // Winner gets 100 points
                LOG_INFO("Player " + std::to_string(player->GetID() + 1) + " finished 1st - awarded 100 points");
            } else {
                player->AddPoints(50); // Second place gets 50 points
                LOG_INFO("Player " + std::to_string(player->GetID() + 1) + " finished 2nd - awarded 50 points");
            }
        }
                
        // Unlock next level if player 1 won
        if (winner == 0 && currentLevelID < 3) {
            UnlockLevel(currentLevelID + 1);
        }
        
        LOG_INFO("Race ended - WINNER: Player " + std::to_string(winner + 1));
        
        // Transition to game over screen
        GameEngine::GetInstance().SetState(GameState::GAME_OVER);
    }
}

void LevelManager::PauseRace() {
    // Pausing handled in GameEngine state
}

void LevelManager::ResumeRace() {
    // Resuming handled in GameEngine state
}

void LevelManager::RestartRace() {
    LoadLevel(currentLevelID);
    StartRace();
}

void LevelManager::AddPlayer(int playerID, const std::string& name) {
    auto player = std::make_unique<Player>(playerID, name);
    players.push_back(std::move(player));
    LOG_INFO("Added player: " + name + " (ID: " + std::to_string(playerID) + ")");
}

Player* LevelManager::GetPlayer(int playerID) const {
    if (playerID >= 0 && playerID < (int)players.size()) {
        return players[playerID].get();
    }
    return nullptr;
}

int LevelManager::GetWinner() const {
    // Simple winner determination: player who finished first or is furthest ahead
    if (players.size() < 2) return 0;
    
    if (players[0]->GetCurrentLap() > players[1]->GetCurrentLap()) {
        return 0;
    } else if (players[1]->GetCurrentLap() > players[0]->GetCurrentLap()) {
        return 1;
    } else {
        // Same lap, check checkpoints
        if (players[0]->GetCheckpointsPassed() >= players[1]->GetCheckpointsPassed()) {
            return 0;
        } else {
            return 1;
        }
    }
}

void LevelManager::UnlockLevel(int levelID) {
    if (levelID > 0 && levelID < (int)unlockedLevels.size()) {
        unlockedLevels[levelID - 1] = true;
        LOG_INFO("Unlocked level " + std::to_string(levelID));
    }
}

bool LevelManager::IsLevelUnlocked(int levelID) const {
    if (levelID > 0 && levelID <= (int)unlockedLevels.size()) {
        return unlockedLevels[levelID - 1];
    }
    return false;
}

void LevelManager::UpdateRaceProgress(float deltaTime) {
    raceTime += deltaTime;
    
    // Check if any player finished all laps
    for (auto& player : players) {
        if (player->GetCurrentLap() > currentTrack->GetRequiredLaps()) {
            EndRace();
            return;
        }
    }
}

void LevelManager::UpdatePlayerPositions() {
    // Determine positions based on lap and checkpoints for all players
    if (players.empty()) return;
    
    // Create a list of player indices sorted by race progress
    std::vector<int> rankings;
    for (size_t i = 0; i < players.size(); i++) {
        rankings.push_back(i);
    }
    
    // Sort players by: lap (descending), then checkpoints (descending)
    std::sort(rankings.begin(), rankings.end(), [this](int a, int b) {
        int lapA = players[a]->GetCurrentLap();
        int lapB = players[b]->GetCurrentLap();
        
        if (lapA != lapB) {
            return lapA > lapB; // Higher lap = better position
        }
        
        // Same lap, check checkpoints
        int cpA = players[a]->GetCheckpointsPassed();
        int cpB = players[b]->GetCheckpointsPassed();
        return cpA > cpB; // More checkpoints = better position
    });
    
    // Assign positions based on ranking
    for (size_t i = 0; i < rankings.size(); i++) {
        players[rankings[i]]->SetRacePosition(i + 1); // Position 1,2,3,4,5...
    }
}

void LevelManager::CheckCheckpoints() {
    if (!currentTrack) return;
    
    for (auto& player : players) {
        Vector3 bikePos = player->GetBike()->GetPosition();
        int currentCheckpoint = player->GetCheckpointsPassed();
        
        // Check if player passed the next checkpoint
        if (currentTrack->CheckCheckpoint(player->GetID(), bikePos, currentCheckpoint)) {
            player->SetCheckpointsPassed(currentCheckpoint + 1);
            
            // Check if completed a lap
            if (currentCheckpoint + 1 >= currentTrack->GetTotalCheckpoints()) {
                player->FinishLap(player->GetCurrentLapTime());
                player->SetCheckpointsPassed(0);
            }
        }
    }
}

void LevelManager::CheckCollisions() {
    auto physicsEngine = GameEngine::GetInstance().GetPhysicsEngine();
    
    // Check ALL bike-to-bike collisions (every pair)
    for (size_t i = 0; i < players.size(); i++) {
        for (size_t j = i + 1; j < players.size(); j++) {
            if (players[i]->GetBike() && players[j]->GetBike()) {
                physicsEngine->ResolveCollision(players[i]->GetBike(), players[j]->GetBike());
            }
        }
    }
    
    // Check bike-obstacle collisions
    if (!currentTrack) return;
    
    for (auto& player : players) {
        if (!player->GetBike()) continue;
        
        Vector3 bikePos = player->GetBike()->GetPosition();
        const float BIKE_COLLISION_RADIUS = 2.0f;
        
        for (const auto& obstacle : currentTrack->GetObstacles()) {
            if (obstacle->CheckCollision(bikePos, BIKE_COLLISION_RADIUS)) {
                // Apply obstacle effect (currently none for solid obstacles)
                obstacle->ApplyEffect(player->GetBike());
                
                // Apply proper collision response for all solid obstacles
                ObstacleType obstacleType = obstacle->GetType();
                
                // Calculate push direction away from obstacle
                Vector3 pushDirection = Vector3Subtract(bikePos, obstacle->GetPosition());
                pushDirection.y = 0; // Keep on ground
                
                float distance = Vector3Length(pushDirection);
                if (distance > 0.01f) {
                    pushDirection = Vector3Normalize(pushDirection);
                    
                    // Push bike away from obstacle
                    float pushStrength = 800.0f; // Increased from 500.0f for stronger collision
                    Vector3 pushForce = Vector3Scale(pushDirection, pushStrength);
                    player->GetBike()->ApplyForce(pushForce);
                    
                    // Reduce velocity on collision for realistic bounce
                    Vector3 currentVel = player->GetBike()->GetVelocity();
                    player->GetBike()->SetVelocity(Vector3Scale(currentVel, 0.6f));
                }
            }
        }
    }
}
