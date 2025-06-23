#pragma once
#include "Examples/IExample.h"

namespace Mir {

class ColorTriangleExample : public IExample {
public:
    ColorTriangleExample();
    ~ColorTriangleExample() override;
    
    void setup() override;
    void render() override;
    void cleanup() override;
    const char* getName() const override { return "Colored Triangle"; }
    
private:
    std::unique_ptr<Shader> m_shader;
    unsigned int m_VAO, m_VBO;
};

} // namespace Mir