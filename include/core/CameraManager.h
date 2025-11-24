#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "raylib.h"
#include "raymath.h"

enum class CameraMode {
    FOLLOW_PLAYER1,
    FOLLOW_PLAYER2,
    SPLIT_SCREEN,
    CINEMATIC
};

class CameraManager {
public:
    CameraManager();
    ~CameraManager() = default;

    void Update(float deltaTime);
    void SetMode(CameraMode mode);
    void SetTarget(int playerID, Vector3 targetPosition, Vector3 targetDirection);

    Camera3D GetCamera(int playerID = 0) const;
    CameraMode GetMode() const { return currentMode; }

private:
    void UpdateFollowCamera(Camera3D& camera, Vector3 targetPos, Vector3 targetDir, float deltaTime);
    void UpdateSplitScreen(float deltaTime);

    CameraMode currentMode;

    // Cameras for each player (for split-screen)
    Camera3D camera1;
    Camera3D camera2;

    // Target positions for smooth following
    Vector3 player1TargetPos;
    Vector3 player1TargetDir;
    Vector3 player2TargetPos;
    Vector3 player2TargetDir;

    // Camera settings
    float cameraDistance;
    float cameraHeight;
    float cameraSmoothness;
};

#endif // CAMERAMANAGER_H
