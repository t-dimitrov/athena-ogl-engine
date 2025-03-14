#include "EditorCamera.h"

#include "Core/Event/WindowEvents.h"
#include "Core/Event/MouseEvents.h"
#include "Core/Event/KeyEvents.h"

#include "Core/Input/Input.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Athena
{
    EditorCamera::EditorCamera()
        : Camera(ProjectionType::Perspective)
    {
        RecalculateView();
    }

    EditorCamera::~EditorCamera()
    {
    }

    void EditorCamera::OnUpdate(float deltaTime)
    {
        auto mousePos = Input::GetMousePosition();
        if (_firstMouseMove)
        {
            _lastMousePos = { mousePos.first, mousePos.second };
            _firstMouseMove = false;
        }

        float xoffset = mousePos.first - _lastMousePos.x;
        float yoffset = _lastMousePos.y - mousePos.second;
        _lastMousePos = { mousePos.first, mousePos.second };

        if (!Input::GetKeyPressed(KeyCode::LeftAlt))
        {
            return;
        }

        if (Input::GetKeyPressed(KeyCode::W))
        {
            _cameraPosition += _cameraSpeed * _cameraFront;
        }
        if (Input::GetKeyPressed(KeyCode::S))
        {
            _cameraPosition -= _cameraSpeed * _cameraFront;
        }
        if (Input::GetKeyPressed(KeyCode::A))
        {
            _cameraPosition -= glm::normalize(glm::cross(_cameraFront, _worldUp)) * _cameraSpeed;
        }
        if (Input::GetKeyPressed(KeyCode::D))
        {
            _cameraPosition += glm::normalize(glm::cross(_cameraFront, _worldUp)) * _cameraSpeed;
        }
        if (Input::GetKeyPressed(KeyCode::Q))
        {
            _cameraPosition -= _cameraSpeed * _worldUp;
        }
        if (Input::GetKeyPressed(KeyCode::E))
        {
            _cameraPosition += _cameraSpeed * _worldUp;
        }

        if (Input::GetMouseButtonPressed(MouseCode::Left))
        {
            xoffset *= _mouseSensitivity;
            yoffset *= _mouseSensitivity;

            _yaw += xoffset;
            _pitch += yoffset;

            if (_pitch > 89.0f)
                _pitch = 89.0f;
            if (_pitch < -89.0f)
                _pitch = -89.0f;

            glm::vec3 direction;
            direction.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            direction.y = std::sin(glm::radians(_pitch));
            direction.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            _cameraFront = glm::normalize(direction);
        }

        RecalculateView();
    }

    void EditorCamera::OnEvent(Event& e)
    {
        if (e.GetEventType() == EventType::WindowResize)
        {
            WindowResizeEvent wre = (WindowResizeEvent&)e;
            SetAspectRatio(static_cast<float>(wre.GetWidth()), static_cast<float>(wre.GetHeight()));
        }
    }

    void EditorCamera::OnImGuiRender()
    {
        ImGui::Begin("Editor Camera");
        
        ImGui::DragFloat("Camera Speed", &_cameraSpeed, 0.01f, 0.01f, 10.0f, "%.2f");
        ImGui::DragFloat("Mouse Sensitivity", &_mouseSensitivity, 0.01f, 0.01f, 10.0f, "%.2f");

        if (ImGui::DragFloat3("Position", &_cameraPosition.x, 0.1f))
        {
            RecalculateView();
        }

        if (ImGui::DragFloat("Yaw", &_yaw, 0.1f))
        {
            glm::vec3 direction;
            direction.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            direction.y = std::sin(glm::radians(_pitch));
            direction.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            _cameraFront = glm::normalize(direction);
            RecalculateView();
        }

        if (ImGui::DragFloat("Pitch", &_pitch, 0.1f, -89.0f, 89.0f))
        {
            glm::vec3 direction;
            direction.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            direction.y = std::sin(glm::radians(_pitch));
            direction.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
            _cameraFront = glm::normalize(direction);
            RecalculateView();
        }

        ImGui::Separator();
        OnImGuiRenderProjection();
        ImGui::Separator();

        ImGui::End();
    }

    void EditorCamera::RecalculateView()
    {
        _viewMatrix = glm::lookAt(_cameraPosition, _cameraPosition + _cameraFront, _worldUp);
    }
}
