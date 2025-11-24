#include "level/Obstacle.h"
#include "entities/Bike.h"
#include "utils/Logger.h"

Obstacle::Obstacle(Vector3 position, ObstacleType type, Vector3 size) :
    position(position),
    size(size),
    type(type),
    moveSpeed(2.0f),
    moveDistance(5.0f),
    currentMoveOffset(0.0f),
    modelLoaded(false)
{
    // Set bounding box
    boundingBox.min = Vector3Subtract(position, Vector3Scale(size, 0.5f));
    boundingBox.max = Vector3Add(position, Vector3Scale(size, 0.5f));
    
    // Set move direction for moving obstacles
    moveDirection = {1, 0, 0};
    
    LoadModel();
}

Obstacle::~Obstacle() {
    if (modelLoaded) {
        UnloadModel(model);
    }
}

void Obstacle::LoadModel() {
    // Create different shapes based on obstacle type
    Mesh mesh;
    
    switch (type) {
        case ObstacleType::STATIC_BARRIER:
            mesh = GenMeshCube(size.x, size.y, size.z);
            break;
        case ObstacleType::MOVING_PLATFORM:
            mesh = GenMeshCube(size.x, size.y, size.z);
            break;
        case ObstacleType::SLOW_ZONE:
            mesh = GenMeshPlane(size.x, size.z, 1, 1);
            break;
        case ObstacleType::SPEED_BOOST:
            mesh = GenMeshPlane(size.x, size.z, 1, 1);
            break;
        case ObstacleType::RAMP:
            mesh = GenMeshCube(size.x, size.y, size.z);
            break;
    }
    
    model = LoadModelFromMesh(mesh);
    
    // Set colors based on type
    Color color = GRAY;
    switch (type) {
        case ObstacleType::STATIC_BARRIER: color = RED; break;
        case ObstacleType::MOVING_PLATFORM: color = ORANGE; break;
        case ObstacleType::SLOW_ZONE: color = BLUE; break;
        case ObstacleType::SPEED_BOOST: color = YELLOW; break;
        case ObstacleType::RAMP: color = BROWN; break;
    }
    
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = color;
    modelLoaded = true;
}

void Obstacle::Update(float deltaTime) {
    if (type == ObstacleType::MOVING_PLATFORM) {
        UpdateMoving(deltaTime);
    }
}

void Obstacle::UpdateMoving(float deltaTime) {
    currentMoveOffset += moveSpeed * deltaTime;
    
    // Oscillate back and forth
    float offset = sinf(currentMoveOffset) * moveDistance;
    position.x = position.x + moveDirection.x * offset * deltaTime;
    position.z = position.z + moveDirection.z * offset * deltaTime;
    
    // Update bounding box
    boundingBox.min = Vector3Subtract(position, Vector3Scale(size, 0.5f));
    boundingBox.max = Vector3Add(position, Vector3Scale(size, 0.5f));
}

void Obstacle::Render() const {
    if (!modelLoaded) return;
    
    DrawModel(model, position, 1.0f, WHITE);
    
    // Debug: draw bounding box
    #ifdef DEBUG
    DrawBoundingBox(boundingBox, PURPLE);
    #endif
}

bool Obstacle::CheckCollision(Vector3 bikePosition, float bikeRadius) const {
    // Simple sphere-box collision
    Vector3 closest = {
        fmaxf(boundingBox.min.x, fminf(bikePosition.x, boundingBox.max.x)),
        fmaxf(boundingBox.min.y, fminf(bikePosition.y, boundingBox.max.y)),
        fmaxf(boundingBox.min.z, fminf(bikePosition.z, boundingBox.max.z))
    };
    
    float distance = Vector3Distance(bikePosition, closest);
    return distance < bikeRadius;
}

void Obstacle::ApplyEffect(Bike* bike) const {
    if (!bike) return;
    
    switch (type) {
        case ObstacleType::SLOW_ZONE:
            // Apply slow effect
            bike->ApplyForce({0, 0, 0});
            bike->SetVelocity(Vector3Scale(bike->GetVelocity(), 0.5f));
            break;
            
        case ObstacleType::SPEED_BOOST:
            // Apply speed boost
            bike->ApplySpeedBoost(1.5f, 1.0f);
            break;
            
        case ObstacleType::STATIC_BARRIER:
        case ObstacleType::MOVING_PLATFORM:
        case ObstacleType::RAMP:
            // Collision handled by physics engine
            break;
    }
}
