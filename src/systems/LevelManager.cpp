#include "systems/LevelManager.h"
#include "core/GameEngine.h"
#include "core/InputManager.h"
#include "core/CameraManager.h"
#include "physics/PhysicsEngine.h"
#include "utils/Logger.h"

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
    
    // Create players
    AddPlayer(0, "Player 1");
    AddPlayer(1, "CPU"); // Rename to CPU
    
    // Set Player 2 as AI
    if (players.size() > 1) {
        players[1]->SetAI(true);
    }
}

void LevelManager::LoadLevel(int levelID) {
    currentLevelID = levelID;
    
    // Create new track
    currentTrack = std::make_unique<Track>();
    
    // Load appropriate track based on level ID
    std::string trackName = "track" + std::to_string(levelID);
    currentTrack->LoadTrack(trackName);
    
    // Reset players with starting positions from track
    for (auto& player : players) {
        // Get proper spawn point from track
        Vector3 startPos = currentTrack->GetSpawnPoint(player->GetID());
        Color bikeColor = player->GetID() == 0 ? RED : BLUE;
        
        // Initialize player with bike
        player->Initialize(startPos, bikeColor);
        
        // Reset player race stats
        player->ResetRace();
    }
    
    // Re-set Player 2 as AI (Initialize/ResetRace don't preserve this flag)
    if (players.size() > 1) {
        players[1]->SetAI(true);
    }
    
    LOG_INFO("Loaded level " + std::to_string(levelID));
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
                    player->UpdateAI(deltaTime, targetPos);
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
    
    // For split-screen, render twice (simplified version)
    // In full implementation, would use viewports properly
    
    BeginMode3D(*std::make_unique<Camera3D>(Camera3D{
        {0, 15, -20},
        {0, 0, 0},
        {0, 1, 0},
        45.0f,
        CAMERA_PERSPECTIVE
    }));
    
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
    // Determine positions based on lap and checkpoints
    if (players.size() < 2) return;
    
    int pos1 = 1, pos2 = 2;
    
    if (players[0]->GetCurrentLap() > players[1]->GetCurrentLap()) {
        pos1 = 1; pos2 = 2;
    } else if (players[1]->GetCurrentLap() > players[0]->GetCurrentLap()) {
        pos1 = 2; pos2 = 1;
    } else {
        // Same lap, check checkpoints
        if (players[0]->GetCheckpointsPassed() >= players[1]->GetCheckpointsPassed()) {
            pos1 = 1; pos2 = 2;
        } else {
            pos1 = 2; pos2 = 1;
        }
    }
    
    players[0]->SetRacePosition(pos1);
    players[1]->SetRacePosition(pos2);
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
    
    // Check bike-to-bike collision
    if (players.size() >= 2) {
        physicsEngine->ResolveCollision(players[0]->GetBike(), players[1]->GetBike());
    }
    
    // Check bike-obstacle collisions
    for (auto& player : players) {
        for (const auto& obstacle : currentTrack->GetObstacles()) {
            Vector3 bikePos = player->GetBike()->GetPosition();
            if (obstacle->CheckCollision(bikePos, 2.0f)) {
                obstacle->ApplyEffect(player->GetBike());
            }
        }
    }
}
