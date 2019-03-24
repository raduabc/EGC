#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects
{
	Mesh* CreateTable(std::string name, float width, float height, float ballRadius, glm::vec3 color, glm::vec3 color2, glm::vec3 color3);
	Mesh* CreateBall(std::string name, glm::vec3 base, float radius, glm::vec3 color);
	Mesh* CreateCue(std::string name, float length, float ball, glm::vec3 color);
}
