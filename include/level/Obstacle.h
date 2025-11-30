#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include "raymath.h"

enum class ObstacleType {
    STATIC_BARRIER,
    MOVING_PLATFORM,
    RAMP
};

class Obstacle {
public:
    Obstacle(Vector3 position, ObstacleType type, Vector3 size = {2.0f, 2.0f, 2.0f});
    ~Obstacle();

    void Update(float deltaTime);
    void Render() const;

    // Collision
    bool CheckCollision(Vector3 bikePosition, float bikeRadius) const;
    BoundingBox GetBoundingBox() const { return boundingBox; }

    // Effects
    void ApplyEffect(class Bike* bike) const;

    ObstacleType GetType() const { return type; }
    Vector3 GetPosition() const { return position; }

private:
    void LoadModel();
    void UpdateMoving(float deltaTime);

    Vector3 position;
    Vector3 size;
    ObstacleType type;
    BoundingBox boundingBox;

    // For moving obstacles
    Vector3 moveDirection;
    float moveSpeed;
    float moveDistance;
    float currentMoveOffset;

    Model model;
    bool modelLoaded;
};

#endif // OBSTACLE_H
