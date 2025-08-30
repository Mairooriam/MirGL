#pragma once

namespace Mir {
    enum class MouseState : unsigned int {
        NONE = 0,                   // No button pressed
        MOUSE_1_PRESSED = 1 << 0,   // Left mouse button pressed
        MOUSE_1_RELEASED = 1 << 1,  // Left mouse button released
        MOUSE_2_PRESSED = 1 << 2,   // Right mouse button pressed
        MOUSE_2_RELEASED = 1 << 3,  // Right mouse button released
        MOUSE_3_PRESSED = 1 << 4,   // Middle mouse button pressed
        MOUSE_3_RELEASED = 1 << 5   // Middle mouse button released
    };

    // Operator overloads for MouseState
    inline MouseState operator|(MouseState a, MouseState b) {
        return static_cast<MouseState>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }

    inline MouseState operator&(MouseState a, MouseState b) {
        return static_cast<MouseState>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    inline MouseState operator~(MouseState a) {
        return static_cast<MouseState>(~static_cast<unsigned int>(a));
    }

    struct Mouse {

        // Screen coordinates
        struct Screen {
            double x = 0.0;
            double y = 0.0;
        } screen;

        // Viewport coordinates
        struct Viewport {
            double x = 0.0;
            double y = 0.0;
        } viewport;

        // World coordinates
        struct World {
            double x = 0.0;
            double y = 0.0;
        } world;

        // Current mouse state (bitmask of MouseState)
        MouseState currentState = MouseState::NONE;

        // Update the mouse state
        void updateState(bool leftPressed, bool rightPressed, bool middlePressed) {
            currentState = MouseState::NONE;

            if (leftPressed) {
                currentState = currentState | MouseState::MOUSE_1_PRESSED;
            } else {
                currentState = currentState | MouseState::MOUSE_1_RELEASED;
            }

            if (rightPressed) {
                currentState = currentState | MouseState::MOUSE_2_PRESSED;
            } else {
                currentState = currentState | MouseState::MOUSE_2_RELEASED;
            }

            if (middlePressed) {
                currentState = currentState | MouseState::MOUSE_3_PRESSED;
            } else {
                currentState = currentState | MouseState::MOUSE_3_RELEASED;
            }
        }

        // Check if a specific state is active
        bool isStateActive(MouseState state) const { return (currentState & state) == state; }
    };

}  // namespace Mir