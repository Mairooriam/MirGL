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

        auto vertexShader = compileShader(GL_VERTEX_SHADER, vShaderResult->c_str(), vertexPath);
        if (!vertexShader) {
            std::cerr << vertexShader.error() << std::endl;
            return;
        }

        auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderResult->c_str(), fragmentPath);
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
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED {}" << infoLog << "\n";
        }

        glDeleteShader(vertexShader.value());
        glDeleteShader(fragmentShader.value());
    }

    Shader::~Shader() {
        if (m_id > 0) glDeleteProgram(m_id);
    }

    void Shader::use() {
        glUseProgram(m_id);
    }

    void Shader::setBool(const std::string& name, bool value) const {
        GLint location = getUniformLocation(name.c_str());
        if (location != -1) {
            glUniform1i(location, (int)value);
        }
    }

    void Shader::setInt(const std::string& name, int value) const {
        GLint location = getUniformLocation(name.c_str());
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    void Shader::setFloat(const std::string& name, float value) const {
        GLint location = getUniformLocation(name.c_str());
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
        GLint location = getUniformLocation(name.c_str());
        if (location != -1) {
            glUniform4fv(location, 1, &value[0]);
        }
    }

    void Shader::setMat4(const char* name, const glm::mat4& mat) const {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
        }
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
        GLint location = getUniformLocation(name.c_str());
        if (location != -1) {
            glUniform3fv(location, 1, &value[0]);
        }
    }

    GLint Shader::getUniformLocation(const char* name) const {
        GLint location = glGetUniformLocation(m_id, name);
        if (location == -1) {
            std::cerr << "WARNING: Uniform '" << name << "' does not exist in shader " << m_id << std::endl;
        }
        return location;
    }

    auto Shader::compileShader(GLenum type, const char* source, const fs::path& filePath)  -> std::expected<unsigned int, std::string> {
        std::string typeStr = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            std::cerr << "ERROR: " << typeStr << " shader compilation failed for file '" << filePath
                      << "':" << std::endl;
            std::cerr << infoLog << std::endl;

            glDeleteShader(shader);
            return std::unexpected(
                std::format("{} shader '{}' compilation failed: {}", typeStr, filePath.string(), infoLog));
        }

        std::cout << typeStr << " shader compiled successfully (ID: " << shader << ")" << " - [" << filePath << "]"
                  << std::endl;
        return shader;
    }
}  // namespace Mir
