#include "Svg.h"

namespace Mir {
    SVG::SVG(const char* file) : m_image{nullptr} {

        m_svgVAO = std::make_unique<VAO>();
        m_svgVBO = std::make_unique<VBO>();
        m_svgPoints.clear();
        m_svgShader = std::make_unique<Shader>("Playground_Simple.vs", "Playground_Simple.fs");

        if (m_image) {
            nsvgDelete(m_image);
            m_image = nullptr;
        }
        
        m_image = nsvgParseFromFile(file, "px", 96.0f);
        if (m_image == nullptr) {
            std::cout << "Could not open SVG image: " << file << std::endl;
            return;
        }

        NSVGshape* shape;
        NSVGpath* path;
        for (shape = m_image->shapes; shape != NULL; shape = shape->next) {
            for (path = shape->paths; path != NULL; path = path->next) {
                setupSVGPoints(path->pts, path->npts, path->closed, 1.0f);  // 1.0f is tolerance
            }
        }

        setupSVGBuffers();
    }
    SVG::~SVG() {
        if (m_image) {
            nsvgDelete(m_image);
            m_image = nullptr;
        }
    }
    void SVG::setupSVGPoints(float* pts, int npts, char closed, float tol) {
        // Add the first point
        if (npts >= 2) {
            float scale = 0.1f;  // Scale factor for your coordinate system
            m_svgPoints.push_back(glm::vec3(pts[0] * scale, -pts[1] * scale, 0.0f));
        }

        // Process cubic bezier curves
        for (int i = 0; i < npts - 1; i += 3) {
            float* p = &pts[i * 2];
            // p[0],p[1] = current point
            // p[2],p[3] = control point 1
            // p[4],p[5] = control point 2
            // p[6],p[7] = end point
            drawCubicBez(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], tol, 0);
        }

        // Close the path if needed
        if (closed && npts >= 2) {
            float scale = 0.1f;
            m_svgPoints.push_back(glm::vec3(pts[0] * scale, -pts[1] * scale, 0.0f));
        }
    }

    void SVG::drawCubicBez(
        float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tol, int level) {
        float x12, y12, x23, y23, x34, y34, x123, y123, x234, y234, x1234, y1234;
        float d;
        float scale = 0.1f;  // Scale factor for your coordinate system

        if (level > 12) return;

        x12 = (x1 + x2) * 0.5f;
        y12 = (y1 + y2) * 0.5f;
        x23 = (x2 + x3) * 0.5f;
        y23 = (y2 + y3) * 0.5f;
        x34 = (x3 + x4) * 0.5f;
        y34 = (y3 + y4) * 0.5f;
        x123 = (x12 + x23) * 0.5f;
        y123 = (y12 + y23) * 0.5f;
        x234 = (x23 + x34) * 0.5f;
        y234 = (y23 + y34) * 0.5f;
        x1234 = (x123 + x234) * 0.5f;
        y1234 = (y123 + y234) * 0.5f;

        d = distPtSeg(x1234, y1234, x1, y1, x4, y4);
        if (d > tol * tol) {
            drawCubicBez(x1, y1, x12, y12, x123, y123, x1234, y1234, tol, level + 1);
            drawCubicBez(x1234, y1234, x234, y234, x34, y34, x4, y4, tol, level + 1);
        } else {
            // Add the end point to our vertex buffer
            m_svgPoints.push_back(glm::vec3(x4 * scale, -y4 * scale, 0.0f));  // Flip Y for OpenGL
        }
    }

    float SVG::distPtSeg(float x, float y, float px, float py, float qx, float qy) {
        float pqx, pqy, dx, dy, d, t;
        pqx = qx - px;
        pqy = qy - py;
        dx = x - px;
        dy = y - py;
        d = pqx * pqx + pqy * pqy;
        t = pqx * dx + pqy * dy;
        if (d > 0) t /= d;
        if (t < 0)
            t = 0;
        else if (t > 1)
            t = 1;
        dx = px + t * pqx - x;
        dy = py + t * pqy - y;
        return dx * dx + dy * dy;
    }

    void SVG::setSVG(const char* file) {
        NSVGimage* tesT_img = nullptr;
        tesT_img = nsvgParseFromFile(file, "px", 96.0f);
        if (tesT_img) {
            std::cout << "test image working" << "\n";
        }

        if (m_image) {
            nsvgDelete(m_image);
            m_image = nsvgParseFromFile(file, "px", 96.0f);
            m_image = nullptr;
        }

        m_svgPoints.clear();

        if (m_image == nullptr) {
            std::cout << "Could not open SVG image: " << file << std::endl;
            return;
        }

        NSVGshape* shape;
        NSVGpath* path;
        for (shape = m_image->shapes; shape != NULL; shape = shape->next) {
            for (path = shape->paths; path != NULL; path = path->next) {
                setupSVGPoints(path->pts, path->npts, path->closed, 1.0f);  // 1.0f is tolerance
            }
        }
    }

    void SVG::setupSVGBuffers() {
        if (m_svgPoints.empty()) {
            return;
        }

        m_svgVAO->bind();
        m_svgVBO->bind();

        glBufferData(GL_ARRAY_BUFFER, m_svgPoints.size() * sizeof(glm::vec3), m_svgPoints.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        m_svgVBO->unbind();
        m_svgVAO->unbind();
    }

}  // namespace Mir