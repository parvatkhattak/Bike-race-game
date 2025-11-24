#include "core/CameraManager.h"
#include "utils/Config.h"

CameraManager::CameraManager() {
    currentMode = CameraMode::SPLIT_SCREEN;
    
    // Initialize camera 1
    camera1.position = {0.0f, 10.0f, 10.0f};
    camera1.target = {0.0f, 0.0f, 0.0f};
    camera1.up = {0.0f, 1.0f, 0.0f};
    camera1.fovy = 45.0f;
    camera1.projection = CAMERA_PERSPECTIVE;
    
    // Initialize camera 2
    camera2 = camera1;
    
    // Settings
    cameraDistance = Config::CAMERA_DISTANCE;
    cameraHeight = Config::CAMERA_HEIGHT;
    cameraSmoothness = Config::CAMERA_SMOOTHNESS;
    
    player1TargetPos = {0, 0, 0};
    player1TargetDir = {0, 0, 1};
    player2TargetPos = {0, 0, 0};
    player2TargetDir = {0, 0, 1};
}

void CameraManager::Update(float deltaTime) {
    switch (currentMode) {
        case CameraMode::FOLLOW_PLAYER1:
            UpdateFollowCamera(camera1, player1TargetPos, player1TargetDir, deltaTime);
            break;
            
        case CameraMode::FOLLOW_PLAYER2:
            UpdateFollowCamera(camera2, player2TargetPos, player2TargetDir, deltaTime);
            break;
            
        case CameraMode::SPLIT_SCREEN:
            UpdateSplitScreen(deltaTime);
            break;
            
        case CameraMode::CINEMATIC:
            // TODO: Implement cinematic camera for victory screen
            break;
    }
}

void CameraManager::SetMode(CameraMode mode) {
    currentMode = mode;
}

void CameraManager::SetTarget(int playerID, Vector3 targetPosition, Vector3 targetDirection) {
    if (playerID == 0) {
        player1TargetPos = targetPosition;
        player1TargetDir = Vector3Normalize(targetDirection);
    } else {
        player2TargetPos = targetPosition;
        player2TargetDir = Vector3Normalize(targetDirection);
    }
}

Camera3D CameraManager::GetCamera(int playerID) const {
    return (playerID == 0) ? camera1 : camera2;
}

void CameraManager::UpdateFollowCamera(Camera3D& camera, Vector3 targetPos, Vector3 targetDir, float deltaTime) {
    // Calculate desired camera position behind the bike
    Vector3 offset = Vector3Scale(targetDir, -cameraDistance);
    offset.y = cameraHeight;
    
    Vector3 desiredPosition = Vector3Add(targetPos, offset);
    
    // Smooth interpolation
    camera.position = Vector3Lerp(camera.position, desiredPosition, cameraSmoothness);
    
    // Look at target with slight offset upward
    Vector3 lookTarget = targetPos;
    lookTarget.y += 2.0f;
    camera.target = Vector3Lerp(camera.target, lookTarget, cameraSmoothness * 1.5f);
}

void CameraManager::UpdateSplitScreen(float deltaTime) {
    // Update both cameras independently
    UpdateFollowCamera(camera1, player1TargetPos, player1TargetDir, deltaTime);
    UpdateFollowCamera(camera2, player2TargetPos, player2TargetDir, deltaTime);
}
