#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Athena
{
    struct DirectionalLight
    {
        float ambientStrength = 0.1f;
        glm::vec3 direction = { -1.0f, -1.0f, -1.0f };
        glm::vec3 color = { 1.0f, 1.0f, 1.0f };

        glm::mat4 orthogonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);

        inline glm::mat4 GetView() const
        {
            return glm::lookAt(50.0f * glm::normalize(-direction), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
        }
    };
}
