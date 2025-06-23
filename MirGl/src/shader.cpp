#include "shader.h"

#include <expected>
#include <fstream>
#include <iostream>

#include "log.h"

std::expected<std::string, std::string> readFile(const fs::path& path) {
    if (!fs::exists(path)) {
        return std::unexpected(std::format("File doesn't exist: {}", path.string()));
    }

    try {
        std::ifstream file(path, std::ios::in);
        if (!file) {
            return std::unexpected(std::format("Failed to open file: {}", path.string()));
        }

        return std::string(std::istreambuf_iterator<char>{file}, {});
    } catch (const std::exception& e) {
        return std::unexpected(std::format("Error reading file {}: {}", path.string(), e.what()));
    }
}

namespace Mir {

    Shader::Shader(fs::path vertexPath, fs::path fragmentPath) {
        auto vShaderResult = readFile(vertexPath);
        auto fShaderResult = readFile(fragmentPath);

        if (!vShaderResult) {
            std::cerr << "Vertex shader error: " << vShaderResult.error() << std::endl;
            return;
        }

        if (!fShaderResult) {
            std::cerr << "Fragment shader error: " << fShaderResult.error() << std::endl;
            return;
        }

        auto vertexShader = compileShader(GL_VERTEX_SHADER, vShaderResult->c_str());
        if (!vertexShader) {
            std::cerr << vertexShader.error() << std::endl;
            return;
        }

        auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderResult->c_str());
        if (!fragmentShader) {
            std::cerr << fragmentShader.error() << std::endl;
            glDeleteShader(*vertexShader);
            return;
        }

        int success;
        char infoLog[512];

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader.value());
        glAttachShader(m_id, fragmentShader.value());
        glLinkProgram(m_id);

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED {}" <<  infoLog << "\n";
        }

        glDeleteShader(vertexShader.value());
        glDeleteShader(fragmentShader.value());
    }

    Shader::~Shader() {
        if (m_id > 0)
            glDeleteProgram(m_id);
    }

    void Shader::use() {
        glUseProgram(m_id);
    }

    void Shader::setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }
    void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    auto Shader::compileShader(GLenum type, const char* source)
        -> std::expected<unsigned int, std::string> {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            std::string typeStr = (type == GL_VERTEX_SHADER)     ? "Vertex"
                                  : (type == GL_FRAGMENT_SHADER) ? "Fragment"
                                                                 : "Unknown";

            glDeleteShader(shader);
            return std::unexpected(
                std::format("{} shader compilation failed: {}", typeStr, infoLog));
        }

        return shader;
    }
}  // namespace Mir
