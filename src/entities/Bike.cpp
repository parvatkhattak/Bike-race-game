#include "entities/Bike.h"
#include "utils/Config.h"
#include "utils/Logger.h"
#include <cmath>

Bike::Bike() :
    position({0, 0, 0}),
    velocity({0, 0, 0}),
    direction({0, 0, 1}),
    rotation(0.0f),
    onGround(true),
    acceleration({0, 0, 0}),
    isBoosted(false),
    boostMultiplier(1.0f),
    boostTimer(0.0f),
    color(WHITE),
    modelLoaded(false)
{
    // Initialize base stats
    baseStats.maxSpeed = Config::BIKE_BASE_SPEED;
    baseStats.acceleration = Config::BIKE_BASE_ACCELERATION;
    baseStats.turnRate = Config::BIKE_BASE_TURN_RATE;
    baseStats.brakeForce = Config::BIKE_BASE_BRAKE_FORCE;
    baseStats.handling = 1.0f;
    baseStats.weight = 1.0f;
    
    stats = baseStats;
}

Bike::~Bike() {
    if (modelLoaded) {
        UnloadModel(model);
    }
}

void Bike::Initialize(Vector3 startPosition, Color bikeColor) {
    position = startPosition;
    color = bikeColor;
    velocity = {0, 0, 0};
    direction = {0, 0, 1};
    rotation = 0.0f;
    
    LoadModel();
}

void Bike::LoadModel() {
    // Create a simple bike model using basic shapes
    // For now, we'll use a cube as a placeholder
    Mesh cubeMesh = GenMeshCube(2.0f, 1.0f, 3.5f);
    model = LoadModelFromMesh(cubeMesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = color;
    modelLoaded = true;
}

void Bike::Update(float deltaTime) {
    UpdatePhysics(deltaTime);
    UpdateRotation(deltaTime);
    
    // Update boost timer
    if (isBoosted) {
        boostTimer -= deltaTime;
        if (boostTimer <= 0.0f) {
            isBoosted = false;
            boostMultiplier = 1.0f;
        }
    }
}

void Bike::UpdatePhysics(float deltaTime) {
    // Apply forces to velocity
    velocity = Vector3Add(velocity, Vector3Scale(acceleration, deltaTime));
    
    // Apply speed limit (with boost multiplier)
    float currentSpeed = Vector3Length(velocity);
    float maxSpeedWithBoost = stats.maxSpeed * boostMultiplier;
    
    if (currentSpeed > maxSpeedWithBoost) {
        velocity = Vector3Scale(Vector3Normalize(velocity), maxSpeedWithBoost);
    }
    
    // Update position
    position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
    
    // Reset acceleration for next frame
    acceleration = {0, 0, 0};
}

void Bike::UpdateRotation(float deltaTime) {
    // Update direction based on velocity
    if (Vector3Length(velocity) > 0.1f) {
        Vector3 velDir = Vector3Normalize(velocity);
        direction = Vector3 Lerp(direction, velDir, 0.1f);
        
        // Calculate rotation angle from direction
        rotation = atan2f(direction.x, direction.z) * RAD2DEG;
    }
}

void Bike::Render() const {
    if (!modelLoaded) return;
    
    // Calculate rotation matrix
    Matrix rotationMatrix = MatrixRotateY(rotation * DEG2RAD);
    Matrix translationMatrix = MatrixTranslate(position.x, position.y, position.z);
    Matrix transform = MatrixMultiply(rotationMatrix, translationMatrix);
    
    // Draw model
    DrawModel(model, position, 1.0f, color);
    
    // Draw debug info (velocity vector)
    #ifdef DEBUG
    Vector3 endPoint = Vector3Add(position, Vector3Scale(velocity, 0.3f));
    DrawLine3D(position, endPoint, GREEN);
    #endif
}

void Bike::Accelerate(float amount) {
    if (amount <= 0.0f) return;
    
    // Apply acceleration in current direction
    Vector3 accelForce = Vector3Scale(direction, stats.acceleration * amount);
    acceleration = Vector3Add(acceleration, accelForce);
}

void Bike::Brake(float amount) {
    if (amount <= 0.0f) return;
    
    // Apply brake force opposite to velocity
    if (Vector3Length(velocity) > 0.1f) {
        Vector3 brakeForce = Vector3Scale(Vector3Normalize(velocity), -stats.brakeForce * amount);
        acceleration = Vector3Add(acceleration, brakeForce);
    }
}

void Bike::Turn(float direction) {
    // direction: -1.0 (left) to 1.0 (right)
    if (fabsf(direction) < 0.01f) return;
    
    float currentSpeed = Vector3Length(velocity);
    if (currentSpeed < 1.0f) return; // Can't turn if not moving
    
    // Turn rate affected by speed and handling
    float speedFactor = fminf(currentSpeed / stats.maxSpeed, 1.0f);
    float turnAmount = direction * stats.turnRate * speedFactor * stats.handling * GetFrameTime();
    
    // Rotate direction vector
    float angleRad = turnAmount * DEG2RAD;
    float newX = this->direction.x * cosf(angleRad) + this->direction.z * sinf(angleRad);
    float newZ = -this->direction.x * sinf(angleRad) + this->direction.z * cosf(angleRad);
    
    this->direction = Vector3Normalize({newX, 0, newZ});
    
    // Also rotate velocity for immediate turn response
    float velX = velocity.x * cosf(angleRad) + velocity.z * sinf(angleRad);
    float velZ = -velocity.x * sinf(angleRad) + velocity.z * cosf(angleRad);
    velocity.x = velX;
    velocity.z = velZ;
}

void Bike::ApplyForce(Vector3 force) {
    acceleration = Vector3Add(acceleration, force);
}

void Bike::ApplyUpgrade(const std::string& upgradeType, int level) {
    float multiplier = 1.0f + (level * 0.1f); // 10% per level
    
    if (upgradeType == "speed") {
        stats.maxSpeed = baseStats.maxSpeed * multiplier;
    } else if (upgradeType == "acceleration") {
        stats.acceleration = baseStats.acceleration * multiplier;
    } else if (upgradeType == "handling") {
        stats.handling = baseStats.handling * multiplier;
        stats.turnRate = baseStats.turnRate * multiplier;
    }
    
    LOG_INFO("Applied upgrade: " + upgradeType + " level " + std::to_string(level));
}

void Bike::ApplySpeedBoost(float multiplier, float duration) {
    isBoosted = true;
    boostMultiplier = multiplier;
    boostTimer = duration;
}
