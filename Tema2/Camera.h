#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace CameraTema
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 2, 5);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 1;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Update camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void aimView(float x, float y, float z) {
				Set(glm::vec3(x, y + 0.5, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				MoveForward(-1);
				Set(this->position, glm::vec3(x, y, z), glm::vec3(0, 1, 0));
			}

			void specView() {
				Set(glm::vec3(0, 6, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
			}

			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				this->position = this->position + dir * distance;
			}

			void RotateFirstPerson_OY(float angle)
			{	
				forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0)));
				right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0)));
				up = glm::cross(right, forward);

			}

			void RotateThirdPerson_OY(float angle)
			{
				// Rotate the camera in Third Person mode - OY axis
				MoveForward(distanceToTarget);
				RotateFirstPerson_OY(angle);
				MoveForward(-distanceToTarget);

			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}