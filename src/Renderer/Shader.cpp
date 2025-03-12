#include "Shader.h"
#include "Core/Log.h"
#include "Core/Core.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace Athena
{
    Shader::Shader(const std::string& vertexSrcFile, const std::string& fragmentSrcFile)
        : _vertexSrcFile(vertexSrcFile)
        , _fragmentSrcFile(fragmentSrcFile)
        , _programId(0)
    {
        const std::string vertexShaderSource = ReadSourceFile(_vertexSrcFile);
        const std::string fragmentShaderSource = ReadSourceFile(_fragmentSrcFile);

        uint32_t vertexShader = CompileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
        uint32_t fragmentShader = CompileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
    
        _programId = CreateProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(_programId);
    }

    std::string Shader::ReadSourceFile(const std::string& filename)
    {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();

        ATH_ASSERT(!buffer.str().empty(), "Shader source filepath '{0}' failed to load", filename);
        
        return buffer.str();
    }

    uint32_t Shader::CompileShader(const char* shaderSource, GLenum shaderType)
    {
        uint32_t shader = glCreateShader(shaderType);

        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            LOG_ERROR("Shader compilation failed for shader type ({0}): {1}", shaderType, infoLog);

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    uint32_t Shader::CreateProgram(uint32_t vertexShader, uint32_t fragmentShader)
    {
        ATH_ASSERT(vertexShader != 0, "Invalid vertex shader!");
        ATH_ASSERT(fragmentShader != 0, "Invalid fragment shader!");

        uint32_t program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            LOG_ERROR("Shader program linking failed: {}", infoLog);
            glDeleteProgram(program);
            return 0;
        }

        return program;
    }

    void Shader::Bind() const
    {
        glUseProgram(_programId);
    }

    void Shader::SetUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::SetUniformInt2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform2i(location, static_cast<GLint>(value.x), static_cast<GLint>(value.y));
    }

    void Shader::SetUniformInt3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform3i(location, static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z));
    }

    void Shader::SetUniformInt4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniform4i(location, static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z), static_cast<GLint>(value.w));
    }

    void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        GLint location = glGetUniformLocation(_programId, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}
