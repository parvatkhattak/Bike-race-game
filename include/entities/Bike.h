#ifndef BIKE_H
#define BIKE_H

#include "raylib.h"
#include "raymath.h"
#include <string>

struct BikeStats {
    float maxSpeed;
    float acceleration;
    float turnRate;
    float brakeForce;
    float handling;
    float weight;
};

class Bike {
public:
    Bike();
    ~Bike();

    void Initialize(Vector3 startPosition, Color bikeColor);
    void Update(float deltaTime);
    void Render() const;

    // Movement
    void Accelerate(float amount);
    void Brake(float amount);
    void Turn(float direction); // -1.0 to 1.0
    void ApplyForce(Vector3 force);

    // Upgrades
    void ApplyUpgrade(const std::string& upgradeType, int level);
    void ApplySpeedBoost(float multiplier, float duration);

    // Getters
    Vector3 GetPosition() const { return position; }
    Vector3 GetVelocity() const { return velocity; }
    Vector3 GetDirection() const { return direction; }
    float GetCurrentSpeed() const { return Vector3Length(velocity); }
    float GetMaxSpeed() const { return stats.maxSpeed; }
    BikeStats GetStats() const { return stats; }
    Model GetModel() const { return model; }
    Color GetColor() const { return color; }

    // Setters
    void SetPosition(Vector3 pos) { position = pos; }
    void SetVelocity(Vector3 vel) { velocity = vel; }
    void SetRotation(float angle) { rotation = angle; }

    // State
    bool IsOnGround() const { return onGround; }
    void SetOnGround(bool grounded) { onGround = grounded; }

private:
    void LoadModel();
    void UpdatePhysics(float deltaTime);
    void UpdateRotation(float deltaTime);

    // Transform
    Vector3 position;
    Vector3 velocity;
    Vector3 direction;
    float rotation; // Rotation around Y-axis in degrees

    // Physics state
    bool onGround;
    Vector3 acceleration;

    // Stats
    BikeStats stats;
    BikeStats baseStats;

    // Boost
    bool isBoosted;
    float boostMultiplier;
    float boostTimer;

    // Visual
    Model model;
    Color color;
    bool modelLoaded;
};

#endif // BIKE_H
