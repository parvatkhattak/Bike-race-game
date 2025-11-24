#include "core/InputManager.h"

void InputManager::Update() {
    // Input is polled per-frame by Raylib automatically
    // This method can be used for any input state processing if needed
}

bool InputManager::IsActionPressed(int playerID, InputAction action) const {
    return CheckKeyboardInput(playerID, action, true) || 
           CheckGamepadInput(playerID, action, true);
}

bool InputManager::IsActionDown(int playerID, InputAction action) const {
    return CheckKeyboardInput(playerID, action, false) || 
           CheckGamepadInput(playerID, action, false);
}

bool InputManager::IsActionReleased(int playerID, InputAction action) const {
    // For released state (not implemented for gamepad yet)
    const KeyMapping& keys = (playerID == 0) ? player1Keys : player2Keys;
    
    switch (action) {
        case InputAction::ACCELERATE:
            return IsKeyReleased(keys.accelerate);
        case InputAction::BRAKE:
            return IsKeyReleased(keys.brake);
        case InputAction::TURN_LEFT:
            return IsKeyReleased(keys.turnLeft);
        case InputAction::TURN_RIGHT:
            return IsKeyReleased(keys.turnRight);
        case InputAction::NITRO:
            return IsKeyReleased(keys.nitro);
        default:
            return false;
    }
}

float InputManager::GetAxisValue(int playerID, InputAction action) const {
    // For turn input, return -1.0 to 1.0
    if (action == InputAction::TURN_LEFT || action == InputAction::TURN_RIGHT) {
        float value = 0.0f;
        
        if (IsActionDown(playerID, InputAction::TURN_LEFT)) {
            value -= 1.0f;
        }
        if (IsActionDown(playerID, InputAction::TURN_RIGHT)) {
            value += 1.0f;
        }
        
        return value;
    }
    
    // For accelerate/brake, return 0.0 to 1.0
    if (action == InputAction::ACCELERATE) {
        return IsActionDown(playerID, InputAction::ACCELERATE) ? 1.0f : 0.0f;
    }
    if (action == InputAction::BRAKE) {
        return IsActionDown(playerID, InputAction::BRAKE) ? 1.0f : 0.0f;
    }
    
    return 0.0f;
}

bool InputManager::IsPausePressed() const {
    return IsKeyPressed(KEY_ESCAPE);
}

bool InputManager::IsConfirmPressed() const {
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
}

bool InputManager::IsCancelPressed() const {
    return IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE);
}

bool InputManager::IsMenuUpPressed() const {
    return IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
}

bool InputManager::IsMenuDownPressed() const {
    return IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
}

bool InputManager::IsMenuLeftPressed() const {
    return IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
}

bool InputManager::IsMenuRightPressed() const {
    return IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
}

bool InputManager::CheckKeyboardInput(int playerID, InputAction action, bool checkPressed) const {
    const KeyMapping& keys = (playerID == 0) ? player1Keys : player2Keys;
    
    switch (action) {
        case InputAction::ACCELERATE:
            return checkPressed ? IsKeyPressed(keys.accelerate) : IsKeyDown(keys.accelerate);
        case InputAction::BRAKE:
            return checkPressed ? IsKeyPressed(keys.brake) : IsKeyDown(keys.brake);
        case InputAction::TURN_LEFT:
            return checkPressed ? IsKeyPressed(keys.turnLeft) : IsKeyDown(keys.turnLeft);
        case InputAction::TURN_RIGHT:
            return checkPressed ? IsKeyPressed(keys.turnRight) : IsKeyDown(keys.turnRight);
        case InputAction::NITRO:
            return checkPressed ? IsKeyPressed(keys.nitro) : IsKeyDown(keys.nitro);
        case InputAction::PAUSE:
            return IsKeyPressed(KEY_ESCAPE);
        default:
            return false;
    }
}

bool InputManager::CheckGamepadInput(int playerID, InputAction action, bool checkPressed) const {
    int gamepadID = playerID; // Gamepad 0 for player 1, Gamepad 1 for player 2
    
    if (!IsGamepadAvailable(gamepadID)) {
        return false;
    }
    
    switch (action) {
        case InputAction::ACCELERATE:
            return IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) ||
                   GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.1f;
        
        case InputAction::BRAKE:
            return IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_TRIGGER_2) ||
                   GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_TRIGGER) > 0.1f;
        
        case InputAction::TURN_LEFT:
            return GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) < -0.2f;
        
        case InputAction::TURN_RIGHT:
            return GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) > 0.2f;
        
        case InputAction::NITRO:
            return checkPressed ? 
                IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) :
                IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        
        default:
            return false;
    }
}
