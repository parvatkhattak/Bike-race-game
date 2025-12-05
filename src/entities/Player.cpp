#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include "entities/Player.h"
#include "core/GameEngine.h"
#include "utils/Logger.h"
#include "raymath.h"

Player::Player(int id, const std::string& name) :
    playerID(id),
    playerName(name),
    isAI(false),
    currentLap(1),
    checkpointsPassed(0),
    currentLapTime(0.0f),
    totalRaceTime(0.0f),
    racePosition(1),
    raceFinished(false)
{
    bike = std::make_unique<Bike>();
    
    // Initialize stats
    stats.totalRacesWon = 0;
    stats.totalRacesPlayed = 0;
    stats.bestLapTime = 999999.0f;
    stats.upgradesUnlocked = 0;
    stats.currentPoints = 0;
}

void Player::Initialize(Vector3 startPosition, Color bikeColor) {
    bike->Initialize(startPosition, bikeColor);
    ResetRace();
}

void Player::Update(float deltaTime) {
    if (!bike) return;
    
    // Update bike
    bike->Update(deltaTime);
    
    // Update race times
    if (!raceFinished) {
        currentLapTime += deltaTime;
        totalRaceTime += deltaTime;
    }
}

void Player::Render() const {
    if (bike) {
        bike->Render();
    }
}

void Player::StartRace() {
    ResetRace();
    LOG_INFO("Player " + std::to_string(playerID) + " (" + playerName + ") started race");
}

void Player::FinishLap(float lapTime) {
    LOG_INFO("Player " + std::to_string(playerID) + " finished lap " + 
             std::to_string(currentLap) + " in " + std::to_string(lapTime) + "s");
    
    // Update best lap time
    if (lapTime < stats.bestLapTime) {
        stats.bestLapTime = lapTime;
    }
    
    // Reset lap timer and checkpoint counter
    currentLapTime = 0.0f;
    checkpointsPassed = 0;
    currentLap++;
}

void Player::FinishRace(int position) {
    raceFinished = true;
    racePosition = position;
    stats.totalRacesPlayed++;
    
    if (position == 1) {
        stats.totalRacesWon++;
        stats.currentPoints += 100; // Winner gets 100 points
    } else if (position == 2) {
        stats.currentPoints += 50; // Second place gets 50 points
    }
    
    LOG_INFO("Player " + std::to_string(playerID) + " finished race in position " + 
             std::to_string(position) + " with time " + std::to_string(totalRaceTime) + "s");
}

void Player::ResetRace() {
    currentLap = 1;
    checkpointsPassed = 0;
    currentLapTime = 0.0f;
    totalRaceTime = 0.0f;
    racePosition = 1;
    raceFinished = false;
    
    // Reset bike velocity
    if (bike) {
        bike->SetVelocity({0, 0, 0});
    }
}

void Player::ProcessInput(float accelerateInput, float brakeInput, float turnInput, bool nitroPressed) {
    if (!bike || raceFinished) return;
    
    // Accelerate
    if (accelerateInput > 0.0f) {
        bike->Accelerate(accelerateInput);
    }
    
    // Brake
    if (brakeInput > 0.0f) {
        bike->Brake(brakeInput);
    }
    
    // Turn
    if (fabsf(turnInput) > 0.01f) {
        bike->Turn(turnInput);
    }
    
    // Nitro boost (if unlocked)
    if (nitroPressed && stats.upgradesUnlocked >= 3) {
        bike->ApplySpeedBoost(1.5f, 2.0f);
    }
}

void Player::UpdateAI(float deltaTime, const Vector3& nextCheckpointPos, int difficulty) {
    if (!bike) return;
    
    Vector3 bikePos = bike->GetPosition();
    Vector3 bikeDir = bike->GetDirection();
    
    // Calculate direction to target
    Vector3 targetDir = Vector3Subtract(nextCheckpointPos, bikePos);
    targetDir.y = 0;
    targetDir = Vector3Normalize(targetDir);
    
    // Calculate alignment with target
    float dot = Vector3DotProduct(bikeDir, targetDir);
    Vector3 cross = Vector3CrossProduct(bikeDir, targetDir);
    
    // SIMPLIFIED AI: Just race forward!
    float accel = 1.0f; // Always full throttle
    float brake = 0.0f;
    float turn = 0.0f;
    bool nitro = false;
    
    // Only turn if significantly off-course
    if (fabsf(cross.y) > 0.15f) {
        turn = (cross.y > 0) ? -0.3f : 0.3f; // Gentle correction
    }
    
    // Use nitro frequently based on difficulty
    int nitroChance = (difficulty == 1) ? 5 : (difficulty == 3) ? 20 : 10;
    if ((rand() % 100) < nitroChance) {
        nitro = true;
    }
    
    ProcessInput(accel, brake, turn, nitro);
}
