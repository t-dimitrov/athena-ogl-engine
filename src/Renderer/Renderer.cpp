#include "Renderer.h"
#include "Core/Log.h"

#include <glad/glad.h>

namespace Athena
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Init()
    {
        _vertexArray = Ref<VertexArray>::Create();
        _vertexArray->Bind();

        float vertices[] = {
             0.5f,  0.5f, 0.0f,   0.0f, 0.5f, 1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 1.0f, 1.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 1.0f, 1.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 0.5f, 1.0f, 1.0f, // top left 
        };
        uint32_t vertexBytes = sizeof(vertices);
        _vertexBuffer = Ref<VertexBuffer>::Create(vertices, vertexBytes);
        _vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
        });

        uint32_t indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        uint32_t indexCount = sizeof(indices) / sizeof(uint32_t);
        _indexBuffer = Ref<IndexBuffer>::Create(indices, indexCount);
        
        _shader = Ref<Shader>::Create("assets/Shaders/Simple.vert.glsl", "assets/Shaders/Simple.frag.glsl");
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _shader->Bind();
        _vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, _indexBuffer->GetElementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
