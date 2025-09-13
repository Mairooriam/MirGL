#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "Primitives.h"


namespace Mir {



    class Light {
      public:
        struct Controls{
            float intensity;
            glm::vec3 color;
        }controls;

        Object mesh;  

        Light(const Controls& control, const Object& meshObj)
            : controls(control), mesh(meshObj) {
        }

        glm::vec3 getLightColor() {
            return controls.color * controls.intensity;
        }
    };
}  // namespace Mir