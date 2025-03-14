#pragma once
#include "Camera.h"
#include "Core/Ref.h"
#include "Core/Event/Event.h"

namespace Athena
{
    class EditorCamera : public Camera, public RefCounted
    {
    public:
        EditorCamera();
        ~EditorCamera();

        void OnUpdate(float deltaTime);
        void OnEvent(Event& e);
        void OnImGuiRender();

        glm::mat4 GetView() const { return _viewMatrix; }

    private:
        void RecalculateView();

    private:
        glm::mat4 _viewMatrix = glm::mat4();

        glm::vec3 _cameraPosition = { 0.0f, 0.0f, 3.0f };
        glm::vec3 _cameraFront = { 0.0f, 0.0f, -1.0f };
        glm::vec3 _worldUp = { 0.0f, 1.0f, 0.0f };

        float _cameraSpeed = 0.1f;
        float _mouseSensitivity = 0.1f;

        bool _firstMouseMove = true;
        glm::vec2 _lastMousePos;
        float _pitch = 0.0f;
        float _yaw = -90.0f;
    };
}
