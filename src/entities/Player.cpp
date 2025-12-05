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
    float currentSpeed = bike->GetSpeed();
    
    // Calculate direction to next checkpoint
    Vector3 targetDir = Vector3Subtract(nextCheckpointPos, bikePos);
    float distanceToTarget = Vector3Length(targetDir);
    targetDir.y = 0; // Ignore height difference for steering
    targetDir = Vector3Normalize(targetDir);
    
    // Calculate steering angle using cross product
    Vector3 cross = Vector3CrossProduct(bikeDir, targetDir);
    float turn = 0.0f;
    
    // Calculate how aligned we are with target (dot product)
    float dot = Vector3DotProduct(bikeDir, targetDir);
    
    // Difficulty-based parameters (more competitive overall)
    float steeringPrecision, accelMult, nitroChance, brakeSpeed;
    
    if (difficulty == 1) {
        // EASY - Still challenging but beatable
        steeringPrecision = 0.07f; // Slightly less precise
        accelMult = 0.9f;          // 90% acceleration (was 75%)
        nitroChance = 8;           // 8% nitro chance (was 3%)
        brakeSpeed = 28.0f;        // Brake a bit later
    } else if (difficulty == 3) {
        // HARD - Very aggressive and competitive
        steeringPrecision = 0.03f; // Very precise steering
        accelMult = 1.2f;          // 120% acceleration (was 110%)
        nitroChance = 25;          // 25% nitro chance (was 20%)
        brakeSpeed = 18.0f;        // Brake very early for perfect corners
    } else {
        // MEDIUM (Level 2) - Strongly competitive
        steeringPrecision = 0.05f;
        accelMult = 1.05f;         // 105% acceleration (was 100%)
        nitroChance = 15;          // 15% nitro chance (was 10%)
        brakeSpeed = 23.0f;        // Brake earlier
    }
    
    // Steering logic with difficulty-based precision
    if (fabsf(cross.y) > steeringPrecision) {
        turn = (cross.y > 0) ? -1.0f : 1.0f;
        
        // Reduce turn rate when closely aligned for smoother racing line
        if (dot > 0.85f) {
            turn *= (difficulty == 1 ? 0.5f : 0.6f); // Easy AI turns less smoothly
        }
    }
    
    // Speed control based on alignment and difficulty
    float accel = 0.0f;
    float brake = 0.0f;
    bool nitro = false;
    
    if (dot > 0.9f) {
        // Nearly perfect alignment - full acceleration
        accel = 1.0f * accelMult;
        
        // Use nitro based on difficulty
        if (dot > 0.95f && currentSpeed > 30.0f && (rand() % 100) < nitroChance) {
            nitro = true;
        }
    } else if (dot > 0.7f) {
        // Good alignment
        accel = 0.8f * accelMult;
    } else if (dot > 0.4f) {
        // Moderate turn
        accel = 0.5f * accelMult;
    } else {
        // Sharp turn ahead - brake to optimal cornering speed
        if (currentSpeed > brakeSpeed) {
            brake = (difficulty == 1 ? 0.6f : 0.8f); // Easy AI brakes less
            accel = 0.0f;
        } else {
            accel = 0.3f * accelMult;
        }
    }
    
    // Anticipatory braking for very sharp turns (harder AI brakes earlier)
    if (dot < 0.3f && currentSpeed > brakeSpeed) {
        brake = (difficulty == 3 ? 1.0f : 0.8f);
        accel = 0.0f;
    }
    
    ProcessInput(accel, brake, turn, nitro);
}
