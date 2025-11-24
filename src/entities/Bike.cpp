#include "entities/Bike.h"
#include "utils/Config.h"
#include "utils/Logger.h"
#include "rlgl.h" // For transformation functions
#include "raymath.h"
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

float Bike::GetSpeed() const {
    return Vector3Length(velocity);
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
        direction = Vector3Lerp(direction, velDir, 0.1f);
        
        // Calculate rotation angle from direction
        rotation = atan2f(direction.x, direction.z) * RAD2DEG;
    }
}

void Bike::Render() const {
    // Save current matrix state
    rlPushMatrix();
    
    // Translate to bike's position
    rlTranslatef(position.x, position.y, position.z);
    
    // Rotate around Y-axis based on bike's direction
    rlRotatef(rotation, 0.0f, 1.0f, 0.0f);
    
    // Draw bike with wheels and body
    Vector3 bodyOffset = {0, 0.5f, 0}; // Body is slightly above ground
    
    // Main bike body (elongated box)
    DrawCube(bodyOffset, 0.6f, 0.8f, 2.0f, color);
    DrawCubeWires(bodyOffset, 0.6f, 0.8f, 2.0f, BLACK);
    
    // Seat
    Vector3 seatOffset = {0, 0.5f + 0.5f, -0.3f};
    DrawCube(seatOffset, 0.5f, 0.3f, 0.6f, ColorBrightness(color, -0.3f));
    
    // Handlebars
    Vector3 handleOffset = {0, 0.5f + 0.3f, 0.8f};
    DrawCube(handleOffset, 1.0f, 0.2f, 0.2f, DARKGRAY);
    
    // Front wheel
    Vector3 frontWheelOffset = {0, 0, 1.2f};
    DrawCylinder(frontWheelOffset, 0.6f, 0.6f, 0.3f, 16, DARKGRAY);
    DrawCylinderWires(frontWheelOffset, 0.6f, 0.6f, 0.3f, 16, BLACK);
    
    // Back wheel  
    Vector3 backWheelOffset = {0, 0, -1.2f};
    DrawCylinder(backWheelOffset, 0.6f, 0.6f, 0.3f, 16, DARKGRAY);
    DrawCylinderWires(backWheelOffset, 0.6f, 0.6f, 0.3f, 16, BLACK);
    
    // Restore matrix state
    rlPopMatrix();
    
    // Draw debug info (velocity vector)
    #ifdef DEBUG
    Vector3 arrowStart = position;
    arrowStart.y += 2.5f;
    Vector3 arrowEnd = Vector3Add(arrowStart, Vector3Scale(direction, 1.5f));
    DrawLine3D(arrowStart, arrowEnd, color);
    DrawSphere(arrowEnd, 0.15f, color);
    
    Vector3 velEndPoint = Vector3Add(position, Vector3Scale(velocity, 0.3f));
    DrawLine3D(position, velEndPoint, GREEN);
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
