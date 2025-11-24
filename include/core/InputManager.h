#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "raylib.h"

enum class InputAction {
    ACCELERATE,
    BRAKE,
    TURN_LEFT,
    TURN_RIGHT,
    NITRO,
    PAUSE,
    CONFIRM,
    CANCEL,
    MENU_UP,
    MENU_DOWN,
    MENU_LEFT,
    MENU_RIGHT
};

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    void Update();

    // Player 1 input (WASD or Gamepad 1)
    bool IsActionPressed(int playerID, InputAction action) const;
    bool IsActionDown(int playerID, InputAction action) const;
    bool IsActionReleased(int playerID, InputAction action) const;
    float GetAxisValue(int playerID, InputAction action) const;

    // General input
    bool IsPausePressed() const;
    bool IsConfirmPressed() const;
    bool IsCancelPressed() const;

    // Menu navigation
    bool IsMenuUpPressed() const;
    bool IsMenuDownPressed() const;
    bool IsMenuLeftPressed() const;
    bool IsMenuRightPressed() const;

private:
    // Helper functions
    bool CheckKeyboardInput(int playerID, InputAction action, bool checkPressed) const;
    bool CheckGamepadInput(int playerID, InputAction action, bool checkPressed) const;

    // Keyboard mappings
    struct KeyMapping {
        KeyboardKey accelerate;
        KeyboardKey brake;
        KeyboardKey turnLeft;
        KeyboardKey turnRight;
        KeyboardKey nitro;
    };

    KeyMapping player1Keys = {KEY_W, KEY_S, KEY_A, KEY_D, KEY_LEFT_SHIFT};
    KeyMapping player2Keys = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_SHIFT};
};

#endif // INPUTMANAGER_H
