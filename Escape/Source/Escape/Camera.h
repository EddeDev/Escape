#pragma once

#include "PrimitiveTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Escape {

	struct Camera
	{
	public:
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 InverseViewProjectionMatrix = glm::mat4(1.0f);
	private:
		glm::vec2 Position = glm::vec2(0.0f);
		float ZoomLevel = 1.0f;
		float AspectRatio = 0.0f;
	public:
		Camera() = default;
		Camera(int32 width, int32 height)
		{
			SetViewportSize(width, height);
		}
	private:
		void RecalculateProjectionMatrix()
		{
			ProjectionMatrix = glm::ortho(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel, -1.0f, 1.0f);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			InverseViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
		}

		void RecalculateViewMatrix()
		{
			ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f)));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			InverseViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
		}
	public:
		void SetViewportSize(int32 width, int32 height)
		{
			AspectRatio = (float)width / (float)height;
			RecalculateProjectionMatrix();
		}

		void SetPosition(const glm::vec2& position)
		{
			Position = position;
			RecalculateViewMatrix();
		}

		glm::vec2 GetPosition() const
		{
			return Position;
		}

		void SetZoomLevel(float zoomLevel)
		{
			ZoomLevel = zoomLevel;
			RecalculateProjectionMatrix();
		}

		float GetZoomLevel() const
		{
			return ZoomLevel;
		}

		float GetAspectRatio() const
		{
			return AspectRatio;
		}
	};

}