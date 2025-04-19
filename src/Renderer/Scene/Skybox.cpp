#include "Skybox.h"

namespace Athena
{
    Skybox::Skybox(const std::vector<std::string>& faces)
    {
        ATH_ASSERT(faces.size() == 6, "There are not 6 faces in this cubemap ({})!", faces.size());
        _cubemap = Ref<Cubemap>::Create(faces);

        _shader = Ref<Shader>::Create("assets/Shaders/Skybox.vert.glsl", "assets/Shaders/Skybox.frag.glsl");
        _vertexArrayObject = Ref<VertexArray>::Create();
        _vertexArrayObject->Bind();

        float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f 
        };
        _vertexBuffer = Ref<VertexBuffer>::Create(cubeVertices, static_cast<uint32_t>(sizeof(cubeVertices)));
        _vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
        });

        uint32_t cubeIndices[36] = {
            0, 1, 2, 2, 3, 0,  // back
            4, 5, 6, 6, 7, 4,  // front
            0, 3, 7, 7, 4, 0,  // left
            1, 5, 6, 6, 2, 1,  // right
            0, 1, 5, 5, 4, 0,  // bottom
            3, 2, 6, 6, 7, 3   // top
        };
        _indexBuffer = Ref<IndexBuffer>::Create(cubeIndices, 36);

        _vertexArrayObject->Unbind();
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::Bind() const
    {
        _shader->Bind();
        _vertexArrayObject->Bind();
    }
}
