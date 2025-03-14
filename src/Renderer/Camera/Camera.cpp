#include "Camera.h"
#include "Core/Log.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Athena
{
	Camera::Camera(ProjectionType projectionType)
		: _projectionType(projectionType)
	{
		RecalculateProjection();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPerspective(float fovy, float viewportWidth, float viewportHeight, float zNear, float zFar)
	{
		_projectionType = ProjectionType::Perspective;

		_FOV = fovy;
		_aspectRatio = viewportWidth / viewportHeight;
		_viewportWidth = viewportWidth;
		_viewportHeight = viewportHeight;
		_near = zNear;
		_far = zFar;

		RecalculateProjection();
	}

	void Camera::SetOrthographic(float left, float right, float bottom, float top)
	{
		_projectionType = ProjectionType::Orthographic;

		_left = left;
		_right = right;
		_bottom = bottom;
		_top = top;

		RecalculateProjection();
	}

	void Camera::SetAspectRatio(float viewportWidth, float viewportHeight)
	{
		_aspectRatio = viewportWidth / viewportHeight;
		_viewportWidth = viewportWidth;
		_viewportHeight = viewportHeight;

		_left = -viewportWidth / 2.0f;
		_right = viewportWidth / 2.0f;
		_bottom = -viewportHeight / 2.0f;
		_top = viewportHeight / 2.0f;

		RecalculateProjection();
	}

	void Camera::RecalculateProjection()
	{
		switch (_projectionType)
		{
			case ProjectionType::Perspective:
			{
				_projection = glm::perspective(glm::radians(_FOV), _aspectRatio, _near, _far);
				break;
			}
			case ProjectionType::Orthographic:
			{
				_projection = glm::ortho(_left, _right, _bottom, _top);
				break;
			}
			default:
			{
				LOG_WARN("Cannot recalculate projection of ProjectionType::None");
				break;
			}
		}
	}

	void Camera::OnImGuiRenderProjection()
	{
		int projectionType = static_cast<int>(_projectionType);
		const char* projectionTypeItems[2] = { "Perspective", "Orthographic" };
		if (ImGui::Combo("Projection Type", 
			&projectionType, 
			projectionTypeItems,
			IM_ARRAYSIZE(projectionTypeItems)))
		{
			_projectionType = static_cast<ProjectionType>(projectionType);
			RecalculateProjection();
		}

		if (_projectionType == ProjectionType::Perspective)
		{
			if (ImGui::DragFloat("Field of View", &_FOV))
			{
				RecalculateProjection();
			}

			if (ImGui::DragFloat("Near", &_near))
			{
				RecalculateProjection();
			}

			if (ImGui::DragFloat("Far", &_far))
			{
				RecalculateProjection();
			}

			ImGui::LabelText("Aspect Ratio", std::to_string(_aspectRatio).c_str());
			ImGui::LabelText("Viewport Width", std::to_string(_viewportWidth).c_str());
			ImGui::LabelText("Viewport Height", std::to_string(_viewportHeight).c_str());
		}
		else if (_projectionType == ProjectionType::Orthographic)
		{
			if (ImGui::DragFloat("Left", &_left))
			{
				RecalculateProjection();
			}

			if (ImGui::DragFloat("Right", &_right))
			{
				RecalculateProjection();
			}

			if (ImGui::DragFloat("Bottom", &_bottom))
			{
				RecalculateProjection();
			}

			if (ImGui::DragFloat("Top", &_top))
			{
				RecalculateProjection();
			}
		}
	}
}
