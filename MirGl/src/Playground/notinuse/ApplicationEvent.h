#pragma once

namespace Mir {
    namespace Event {
        enum class ApplicationType {
            WindowResize,
            WindowClose,
            AppTick,
            AppUpdate,
            AppRender
        };

        struct WindowResize {
            ApplicationType type = ApplicationType::WindowResize;
            unsigned int width, height;
            bool handled = false;

            WindowResize(unsigned int width, unsigned int height) 
                : width(width), height(height) {}
        };

        struct WindowClose {
            ApplicationType type = ApplicationType::WindowClose;
            bool handled = false;

            WindowClose() = default;
        };

        struct AppTick {
            ApplicationType type = ApplicationType::AppTick;
            bool handled = false;

            AppTick() = default;
        };

        struct AppUpdate {
            ApplicationType type = ApplicationType::AppUpdate;
            bool handled = false;

            AppUpdate() = default;
        };

        struct AppRender {
            ApplicationType type = ApplicationType::AppRender;
            bool handled = false;

            AppRender() = default;
        };
    }
}