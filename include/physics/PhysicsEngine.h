#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "raylib.h"
#include "raymath.h"
#include "../entities/Bike.h"
#include <vector>

struct Collider {
    Vector3 center;
    float radius;
    bool isStatic;
};

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine() = default;

    void Update(float deltaTime);
    void ApplyPhysics(Bike* bike, float deltaTime);

    // Collision detection
    bool CheckSphereCollision(Vector3 pos1, float radius1, Vector3 pos2, float radius2) const;
    bool CheckAABBCollision(BoundingBox box1, BoundingBox box2) const;
    bool CheckGroundCollision(Vector3 position, float& groundHeight) const;

    // Collision response
    void ResolveCollision(Bike* bike1, Bike* bike2);
    void ResolveStaticCollision(Bike* bike, Collider staticCollider);

    // Environment
    void SetGroundHeight(float height) { groundHeight = height; }
    float GetGroundHeight() const { return groundHeight; }
    void SetGravity(float g) { gravity = g; }

    // Colliders management
    void AddStaticCollider(Vector3 center, float radius);
    void ClearColliders();

private:
    void ApplyGravity(Bike* bike, float deltaTime);
    void ApplyFriction(Bike* bike, float deltaTime);
    void ApplyDrag(Bike* bike, float deltaTime);
    void KeepOnTrack(Bike* bike);

    float gravity;
    float groundHeight;
    float frictionCoefficient;
    float dragCoefficient;

    std::vector<Collider> staticColliders;
};

#endif // PHYSICSENGINE_H
