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

void Player::UpdateAI(float deltaTime, const Vector3& nextCheckpointPos) {
    if (!bike) return;
    
    Vector3 bikePos = bike->GetPosition();
    Vector3 bikeDir = bike->GetDirection();
    
    // Calculate direction to next checkpoint
    Vector3 targetDir = Vector3Subtract(nextCheckpointPos, bikePos);
    targetDir.y = 0; // Ignore height difference for steering
    targetDir = Vector3Normalize(targetDir);
    
    // Calculate steering angle using cross product
    // Cross product Y component tells us left/right
    Vector3 cross = Vector3CrossProduct(bikeDir, targetDir);
    float turn = 0.0f;
    
    // Steering logic
    if (cross.y > 0.1f) turn = -1.0f; // Turn Left
    else if (cross.y < -0.1f) turn = 1.0f; // Turn Right
    
    // Adjust turn sensitivity based on how far off we are
    float dot = Vector3DotProduct(bikeDir, targetDir);
    
    // Speed control
    float accel = 0.0f;
    float brake = 0.0f;
    bool nitro = false;
    
    if (dot > 0.8f) {
        // Facing mostly towards target, full speed
        accel = 1.0f;
        // Use nitro on straightaways occasionally
        if (dot > 0.95f && (rand() % 100) < 2) nitro = true; 
    } else if (dot > 0.5f) {
        // Turning, moderate speed
        accel = 0.5f;
    } else {
        // Sharp turn, brake
        brake = 1.0f;
        // Sharp turn
        turn = (cross.y > 0) ? -1.0f : 1.0f;
    }
    
    ProcessInput(accel, brake, turn, nitro);
}
