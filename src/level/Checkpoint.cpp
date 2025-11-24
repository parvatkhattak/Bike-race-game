#include "level/Checkpoint.h"

Checkpoint::Checkpoint(Vector3 position, float radius, int id) :
    position(position),
    radius(radius),
    checkpointID(id),
    isActive(true)
{
}

bool Checkpoint::CheckPassage(Vector3 bikePosition, float bikeRadius) const {
    if (!isActive) return false;
    
    float distance = Vector3Distance(position, bikePosition);
    return distance < (radius + bikeRadius);
}

void Checkpoint::Render() const {
    // Debug visualization
    DrawCircle3D(position, radius, {1, 0, 0}, 90.0f, isActive ? GREEN : RED);
    DrawCylinder(position, radius, radius, 0.2f, 16, isActive ? ColorAlpha(GREEN, 0.3f) : ColorAlpha(RED, 0.3f));
}
