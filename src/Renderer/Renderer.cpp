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
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);


        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        _shader = Ref<Shader>::Create("assets/Shaders/Simple.vert.glsl", "assets/Shaders/Simple.frag.glsl");
    }

    void Renderer::Shutdown()
    {
        glDeleteBuffers(1, &_vbo);
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _shader->Bind();
        _shader->SetUniformFloat4("uColor", glm::vec4(0.1f, 0.3f, 0.2f, 1.0f));
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
