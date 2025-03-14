#pragma once
#include <glm/glm.hpp>

namespace Athena
{
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(ProjectionType projectionType);
		~Camera();

		void SetPerspective(float fovy, float viewportWidth, float viewportHeight, float zNear, float zFar);
		void SetOrthographic(float left, float right, float bottom, float top);

		glm::mat4 GetProjection() const { return _projection; }
		float GetAspectRatio() const { return _aspectRatio; }
		float GetViewportWidth() const { return _viewportWidth; }
		float GetViewportHeight() const { return _viewportHeight; }

		float GetNear() const { return _near; }
		float GetFar() const { return _far; }

		float GetFOV() const { return _FOV; }

		void SetAspectRatio(float viewportWidth, float viewportHeight);

	private:
		void RecalculateProjection();
	
	protected:
		void OnImGuiRenderProjection();

	protected:
		ProjectionType _projectionType = ProjectionType::Perspective;
		glm::mat4 _projection;

		// Perspective
		float _FOV = 45.0f;
		float _aspectRatio = 16.0f / 9.0f;
		float _viewportWidth = 1280.0f;
		float _viewportHeight = 720.0f;
		float _near = 0.01f;
		float _far = 1000.0f;

		// Orthographic
		float _left = -800;
		float _right = 800;
		float _bottom = -450;
		float _top = 450;
	};
}