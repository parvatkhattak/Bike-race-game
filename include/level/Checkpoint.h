#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "raylib.h"
#include "raymath.h"

class Checkpoint {
public:
    Checkpoint(Vector3 position, float radius, int id);
    ~Checkpoint() = default;

    bool CheckPassage(Vector3 bikePosition, float bikeRadius) const;
    void Render() const; // For debug visualization

    Vector3 GetPosition() const { return position; }
    int GetID() const { return checkpointID; }
    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }

private:
    Vector3 position;
    float radius;
    int checkpointID;
    bool isActive;
};

#endif // CHECKPOINT_H
