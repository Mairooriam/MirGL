#pragma once

namespace Mir {
    namespace Event {
        enum class KeyType {
            KeyPressed,
            KeyReleased,
            KeyTyped
        };

        struct KeyPressed {
            KeyType type = KeyType::KeyPressed;
            int keyCode;
            int repeatCount;
            bool handled = false;

            KeyPressed(int keyCode, int repeatCount) 
                : keyCode(keyCode), repeatCount(repeatCount) {}
        };

        struct KeyReleased {
            KeyType type = KeyType::KeyReleased;
            int keyCode;
            bool handled = false;

            KeyReleased(int keyCode) : keyCode(keyCode) {}
        };

        struct KeyTyped {
            KeyType type = KeyType::KeyTyped;
            int keyCode;
            bool handled = false;

            KeyTyped(int keyCode) : keyCode(keyCode) {}
        };
    }
}