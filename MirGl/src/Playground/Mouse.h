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
        glm::dvec2 screen{0.0, 0.0};
        glm::dvec2 viewport{0.0, 0.0};
        glm::dvec2 world{0.0, 0.0};

        glm::dvec2 screenLast{0.0, 0.0};
        glm::dvec2 viewportLast{0.0, 0.0};
        glm::dvec2 worldLast{0.0, 0.0};

        MouseState currentState = MouseState::NONE;

        void setCurrentLast() {
            screenLast = screen;
            viewportLast = viewport;
            worldLast = world;
        }

        glm::dvec2 getOffset() const { return screen - screenLast; }

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