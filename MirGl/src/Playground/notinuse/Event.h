#pragma once
#include "Playground/KeyEvent.h"
#include "Playground/MouseEvent.h"
#include "Playground/ApplicationEvent.h"
#include <variant>

namespace Mir {
    // Combined event type for all events
    enum class EventCategory {
        None = 0,
        Application = 1 << 0,
        Input = 1 << 1,
        Keyboard = 1 << 2,
        Mouse = 1 << 3
    };

    // Union of all possible events
    using Event = std::variant<
        // Mouse Events
        Event::MouseMoved,
        Event::MouseScrolled,
        Event::MouseButtonPressed,
        Event::MouseButtonReleased,
        // Key Events
        Event::KeyPressed,
        Event::KeyReleased,
        Event::KeyTyped,
        // Application Events
        Event::WindowResize,
        Event::WindowClose,
        Event::AppTick,
        Event::AppUpdate,
        Event::AppRender
    >;

    // Helper functions for event handling
    template<typename T>
    bool IsEventType(const Event& event) {
        return std::holds_alternative<T>(event);
    }

    template<typename T>
    T& GetEvent(Event& event) {
        return std::get<T>(event);
    }

    template<typename T>
    const T& GetEvent(const Event& event) {
        return std::get<T>(event);
    }

    // Event dispatcher using visitor pattern
    class EventDispatcher {
    public:
        template<typename T, typename F>
        static bool Dispatch(Event& event, F&& func) {
            if (std::holds_alternative<T>(event)) {
                return func(std::get<T>(event));
            }
            return false;
        }
    };
}