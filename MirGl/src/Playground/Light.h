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

        std::shared_ptr<Object> mesh;  

        Light(const Controls& control, const glm::vec3& position, const glm::vec3& color, std::shared_ptr<Object> meshObj)
            : controls(control), mesh(meshObj) {
            mesh->setColor(color);
            mesh->setPosition(position);
            
        }
    };
}  // namespace Mir