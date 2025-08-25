#pragma once

namespace Mir {
    namespace Event {
        enum class MouseType {
            MouseMoved,
            MouseScrolled,
            MouseButtonPressed,
            MouseButtonReleased
        };

        struct MouseMoved {
            MouseType type = MouseType::MouseMoved;
            float x, y;
            bool handled = false;

            MouseMoved(float x, float y) : x(x), y(y) {}
        };

        struct MouseScrolled {
            MouseType type = MouseType::MouseScrolled;
            float xOffset, yOffset;
            bool handled = false;

            MouseScrolled(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}
        };

        struct MouseButtonPressed {
            MouseType type = MouseType::MouseButtonPressed;
            int button;
            bool handled = false;

            MouseButtonPressed(int button) : button(button) {}
        };

        struct MouseButtonReleased {
            MouseType type = MouseType::MouseButtonReleased;
            int button;
            bool handled = false;

            MouseButtonReleased(int button) : button(button) {}
        };
    }
}