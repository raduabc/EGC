#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}



Mesh* Object2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;// +glm::vec3(length, length, 0);

	std::vector<VertexFormat> vertices;

	for (int i = 0; i < 360; i += 10)
	{
		float rad = i * 3.14 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(rad) * length, sin(rad) * length, 0), color));
	}

	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i < 36; i++) {
		indices.push_back(i);
		indices.push_back((i + 1) % 36);
		indices.push_back((i + 18) % 36);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}


Mesh * Object2D::CreateHeart(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;// +glm::vec3(length, length, 0);

	std::vector<VertexFormat> vertices = {

		VertexFormat(corner + glm::vec3(-length / 2, length / 2, 0), color), // 0 centrul cerc stanga
		VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color), // 1 centrul cerc dreapta
		VertexFormat(corner + glm::vec3(0, -length, 0), color), // 2 triunghi jos
		VertexFormat(corner + glm::vec3(-length, length / 2, 0), color), // 3 triunghi stanga
		VertexFormat(corner + glm::vec3(length, length / 2, 0), color) // 4 triunghi dreapta
	};
	length /= 2;

	for (int i = 0; i <= 180; i += 180 / 18)
	{
		float rad = i * 3.14 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(rad) * length, sin(rad) * length, 0) + glm::vec3(-length, length, 0), color));
	}
	for (int i = 0; i <= 180; i += 180 / 18)
	{
		float rad = i * 3.14 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(rad) * length, sin(rad) * length, 0) + glm::vec3(length, length, 0), color));
	}


	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices = {2, 3, 4};
	// push back indices
	for (int i = 5; i < 5 + 19; i++) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(2);
	}

	for (int i = 5 + 19; i < 5 + 19 + 19; i++) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(2);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}