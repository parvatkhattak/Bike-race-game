#include "core/CameraManager.h"
#include "utils/Config.h"

CameraManager::CameraManager() {
    currentMode = BikeGameCameraMode::SPLIT_SCREEN;
    
    // Initialize camera 1 - positioned to see starting area
    camera1.position = {0.0f, 25.0f, -35.0f};  // Higher and further back
    camera1.target = {0.0f, 0.0f, 0.0f};
    camera1.up = {0.0f, 1.0f, 0.0f};
    camera1.fovy = 60.0f;
    camera1.projection = CAMERA_PERSPECTIVE;
    
    // Initialize camera 2 - same initial position
    camera2 = camera1;
    
    // Initialize target positions
    player1TargetPos = {-10.0f, 0.0f, 0.0f};
    player1TargetDir = {0.0f, 0.0f, 1.0f};
    player2TargetPos = {10.0f, 0.0f, 0.0f};
    player2TargetDir = {0.0f, 0.0f, 1.0f};
}

void CameraManager::Update(float deltaTime) {
    switch (currentMode) {
        case BikeGameCameraMode::FOLLOW_PLAYER1:
            UpdateFollowCamera(camera1, player1TargetPos, player1TargetDir, deltaTime);
            break;
            
        case BikeGameCameraMode::FOLLOW_PLAYER2:
            UpdateFollowCamera(camera2, player2TargetPos, player2TargetDir, deltaTime);
            break;
            
        case BikeGameCameraMode::SPLIT_SCREEN:
            UpdateSplitScreen(deltaTime);
            break;
            
        case BikeGameCameraMode::CINEMATIC:
            // TODO: Implement cinematic camera for victory screen
            break;
    }
}

void CameraManager::SetMode(BikeGameCameraMode mode) {
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
    // Camera follows from behind and above the bike
    float cameraDistance = 20.0f;  // Further back
    float cameraHeight = 12.0f;     // Higher up
    
    // Calculate desired camera position (behind the bike)
    Vector3 desiredPosition = targetPos;
    desiredPosition.x -= targetDir.x * cameraDistance;
    desiredPosition.z -= targetDir.z * cameraDistance;
    desiredPosition.y += cameraHeight;
    
    // Smoothly interpolate camera position
    float smoothness = 8.0f * deltaTime;  // Faster follow
    camera.position = Vector3Lerp(camera.position, desiredPosition, smoothness);
    
    // Camera looks at bike position slightly ahead
    Vector3 lookAtPoint = targetPos;
    lookAtPoint.y += 2.0f;  // Look slightly above bike
    camera.target = Vector3Lerp(camera.target, lookAtPoint, smoothness);
}

void CameraManager::UpdateSplitScreen(float deltaTime) {
    // Update both cameras independently
    UpdateFollowCamera(camera1, player1TargetPos, player1TargetDir, deltaTime);
    UpdateFollowCamera(camera2, player2TargetPos, player2TargetDir, deltaTime);
}
