#pragma once
#include <glad/glad.h> 
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <expected>
namespace fs = std::filesystem;
namespace Mir
{
    class Shader
    {
    private:
        auto compileShader(GLenum type, const char* source) -> std::expected<unsigned int, std::string>;
    public:
        unsigned int m_id; 
        Shader(fs::path vertexPath, fs::path fragmentPath);
        ~Shader();
        void use();
        void setBool(const std::string&name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
  

    };
    

        
} // namespace Mir
