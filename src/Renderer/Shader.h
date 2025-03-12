#pragma once
#include "Core/Ref.h"

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Athena
{
    class Shader : public RefCounted
    {
    public:
        Shader(const std::string& vertexSrcFile, const std::string& fragmentSrcFile);
        ~Shader();

        void Bind() const;

        void SetUniformFloat(const std::string& name, float value);
        void SetUniformFloat2(const std::string& name, const glm::vec2& value);
        void SetUniformFloat3(const std::string& name, const glm::vec3& value);
        void SetUniformFloat4(const std::string& name, const glm::vec4& value);
        
        void SetUniformInt(const std::string& name, int value);
        void SetUniformInt2(const std::string& name, const glm::vec2& value);
        void SetUniformInt3(const std::string& name, const glm::vec3& value);
        void SetUniformInt4(const std::string& name, const glm::vec4& value);
        
        void SetUniformMat4(const std::string& name, const glm::mat4& value);
    
    private:
        std::string ReadSourceFile(const std::string& filename);
        uint32_t CompileShader(const char* shaderSource, GLenum shaderType);
        uint32_t CreateProgram(uint32_t vertexShader, uint32_t fragmentShader);

    private:
        std::string _vertexSrcFile, _fragmentSrcFile;
        uint32_t _programId;
    };
}
