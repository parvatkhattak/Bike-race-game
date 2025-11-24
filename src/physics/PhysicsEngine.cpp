#include "physics/PhysicsEngine.h"
#include "utils/Config.h"
#include "utils/Logger.h"
#include <cmath>

PhysicsEngine::PhysicsEngine() :
    gravity(Config::GRAVITY),
    groundHeight(0.5f),
    frictionCoefficient(Config::GROUND_FRICTION),
    dragCoefficient(Config::AIR_DRAG)
{
    LOG_INFO("PhysicsEngine initialized");
}

void PhysicsEngine::Update(float deltaTime) {
    // Physics is applied per-bike in ApplyPhysics method
    // This can be used for global physics updates if needed
}

void PhysicsEngine::ApplyPhysics(Bike* bike, float deltaTime) {
    if (!bike) return;
    
    ApplyGravity(bike, deltaTime);
    ApplyFriction(bike, deltaTime);
    ApplyDrag(bike, deltaTime);
    KeepOnTrack(bike);
}

void PhysicsEngine::ApplyGravity(Bike* bike, float deltaTime) {
    Vector3 pos = bike->GetPosition();
    Vector3 vel = bike->GetVelocity();
    
    // Check ground collision
    float groundY;
    bool isGrounded = CheckGroundCollision(pos, groundY);
    
    if (isGrounded) {
        // On ground - set Y position and stop vertical movement
        pos.y = groundY;
        vel.y = 0.0f;
        bike->SetOnGround(true);
    } else {
        // In air - apply gravity
        vel.y -= gravity * deltaTime;
        bike->SetOnGround(false);
    }
    
    bike->SetPosition(pos);
    bike->SetVelocity(vel);
}

void PhysicsEngine::ApplyFriction(Bike* bike, float deltaTime) {
    if (!bike->IsOnGround()) return; // Only apply friction on ground
    
    Vector3 vel = bike->GetVelocity();
    
    // Apply friction to horizontal velocity
    vel.x *= frictionCoefficient;
    vel.z *= frictionCoefficient;
    
    // Stop completely if very slow
    if (Vector3Length(vel) < 0.1f) {
        vel = {0, vel.y, 0};
    }
    
    bike->SetVelocity(vel);
}

void PhysicsEngine::ApplyDrag(Bike* bike, float deltaTime) {
    Vector3 vel = bike->GetVelocity();
    
    // Apply air drag
    vel.x *= dragCoefficient;
    vel.z *= dragCoefficient;
    
    bike->SetVelocity(vel);
}

void PhysicsEngine::KeepOnTrack(Bike* bike) {
    Vector3 pos = bike->GetPosition();
    
    // Simple boundary check (can be enhanced with actual track bounds)
    const float MAX_DISTANCE = 100.0f;
    
    if (fabsf(pos.x) > MAX_DISTANCE || fabsf(pos.z) > MAX_DISTANCE) {
        // Reset to center if out of bounds
        bike->SetPosition({0, groundHeight, 0});
        bike->SetVelocity({0, 0, 0});
        LOG_WARNING("Bike went out of bounds, resetting position");
    }
}

bool PhysicsEngine::CheckSphereCollision(Vector3 pos1, float radius1, Vector3 pos2, float radius2) const {
    float distance = Vector3Distance(pos1, pos2);
    return distance < (radius1 + radius2);
}

bool PhysicsEngine::CheckAABBCollision(BoundingBox box1, BoundingBox box2) const {
    return CheckCollisionBoxes(box1, box2);
}

bool PhysicsEngine::CheckGroundCollision(Vector3 position, float& outGroundHeight) const {
    // Simple ground check - always on ground at groundHeight
    // This can be enhanced with raycasting for varied terrain
    outGroundHeight = groundHeight;
    return position.y <= groundHeight + 0.1f;
}

void PhysicsEngine::ResolveCollision(Bike* bike1, Bike* bike2) {
    if (!bike1 || !bike2) return;
    
    Vector3 pos1 = bike1->GetPosition();
    Vector3 pos2 = bike2->GetPosition();
    
    const float BIKE_RADIUS = 2.0f;
    
    if (CheckSphereCollision(pos1, BIKE_RADIUS, pos2, BIKE_RADIUS)) {
        // Calculate collision normal
        Vector3 normal = Vector3Subtract(pos1, pos2);
        float distance = Vector3Length(normal);
        
        if (distance > 0.0f) {
            normal = Vector3Normalize(normal);
            
            // Separate bikes
            float overlap = (BIKE_RADIUS * 2.0f) - distance;
            Vector3 separation = Vector3Scale(normal, overlap * 0.5f);
            
            bike1->SetPosition(Vector3Add(pos1, separation));
            bike2->SetPosition(Vector3Subtract(pos2, separation));
            
            // Apply bounce to velocities
            Vector3 vel1 = bike1->GetVelocity();
            Vector3 vel2 = bike2->GetVelocity();
            
            Vector3 relativeVel = Vector3Subtract(vel1, vel2);
            float separatingVelocity = Vector3DotProduct(relativeVel, normal);
            
            if (separatingVelocity < 0) {
                float restitution = 0.5f; // Bounciness
                float impulse = (-1 - restitution) * separatingVelocity;
                Vector3 impulseVector = Vector3Scale(normal, impulse * 0.5f);
                
                bike1->SetVelocity(Vector3Add(vel1, impulseVector));
                bike2->SetVelocity(Vector3Subtract(vel2, impulseVector));
            }
        }
    }
}

void PhysicsEngine::ResolveStaticCollision(Bike* bike, Collider staticCollider) {
    if (!bike) return;
    
    Vector3 bikePos = bike->GetPosition();
    const float BIKE_RADIUS = 2.0f;
    
    if (CheckSphereCollision(bikePos, BIKE_RADIUS, staticCollider.center, staticCollider.radius)) {
        // Push bike out of obstacle
        Vector3 normal = Vector3Subtract(bikePos, staticCollider.center);
        normal = Vector3Normalize(normal);
        
        float penetration = (BIKE_RADIUS + staticCollider.radius) - Vector3Distance(bikePos, staticCollider.center);
        Vector3 correction = Vector3Scale(normal, penetration);
        
        bike->SetPosition(Vector3Add(bikePos, correction));
        
        // Dampen velocity in collision direction
        Vector3 vel = bike->GetVelocity();
        float velInNormal = Vector3DotProduct(vel, normal);
        if (velInNormal < 0) {
            Vector3 tangentVel = Vector3Subtract(vel, Vector3Scale(normal, velInNormal));
            bike->SetVelocity(Vector3Scale(tangentVel, 0.7f)); // Lose some speed
        }
    }
}

void PhysicsEngine::AddStaticCollider(Vector3 center, float radius) {
    Collider collider;
    collider.center = center;
    collider.radius = radius;
    collider.isStatic = true;
    staticColliders.push_back(collider);
}

void PhysicsEngine::ClearColliders() {
    staticColliders.clear();
}
