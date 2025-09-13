#pragma once
#include <nanosvg.h>

#include "Opengl/VAO.h"
#include "Opengl/VBO.h"
#include "shader.h"
#include <memory>
#include <vector>
#include <filesystem>
namespace Mir{

    class SVG
    {
    private:
        NSVGimage* m_image = NULL;

    public:
        std::unique_ptr<VAO> m_svgVAO;
        std::unique_ptr<VBO> m_svgVBO;
        std::vector<glm::vec3> m_svgPoints;
        std::unique_ptr<Shader> m_svgShader;
        SVG(const char* file);
        ~SVG();
        void setSVG(const char * file);
        void setupSVGBuffers();
        void setupSVGPoints(float* pts, int npts, char closed, float tol);
        void drawCubicBez(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tol, int level);
        float distPtSeg(float x, float y, float px, float py, float qx, float qy);
    };
    

    
}