#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

namespace Mir {
    struct DragDrop;

    struct Mouse;
    enum class ActiveWindow;
    class Object;
    class Light;
    class Grid;
    struct DebugData {
        // Camera Info

        bool MainWindow = true;
        struct CameraInfo {
            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            float yaw;
            float pitch;
            float zoom;
        };

        CameraInfo perspectiveCamera;
        CameraInfo orthoCamera;

        Mouse* mouse = nullptr;

        // Timing Info
        float deltaTime;
        float time;
        float fps;

        // Window Info
        int windowWidth;
        int windowHeight;
        float aspectRatio;

        // Debug matrices
        glm::mat4 inverseProjection;
        glm::mat4 inverseView;
        glm::vec3 worldPosition;

        // General
        bool showWireframe = false;

        // Objects
        std::vector<Object>* objects;

        // Lights control
        std::vector<Light>* lights = nullptr;

        //
        DragDrop* dragDrop = nullptr;

        Grid* grid = nullptr;
    };
}  // namespace Mir