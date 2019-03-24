#pragma once

#include <include/glm.h>

namespace Transform
{
	// Translate matrix
	inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
	{
		return glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translateX, translateY, translateZ, 1
		);
	}

	// Scale matrix
	inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
	{
		return glm::transpose(
			glm::mat4(
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			)
		);
	}
	inline glm::mat4 RotateOX(float x) {
		return glm::transpose(
			glm::mat4(
				1, 0, 0, 0,
				0, cos(x), -sin(x), 0,
				0, sin(x), cos(x), 0,
				0, 0, 0, 1
			)
		);
	}
	inline glm::mat4 RotateOY(float x) {
		return glm::transpose(
			glm::mat4(
				cos(x), 0, -sin(x), 0,
				0, 1, 0, 0,
				sin(x), 0, cos(x), 0,
				0, 0, 0, 1
			)
		);
	}
	inline glm::mat4 RotateOZ(float x) {
		return glm::transpose(
			glm::mat4(
				cos(x), -sin(x), 0, 0,
				sin(x), cos(x), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			)
		);
	}
}
