#include "Objects.h"

#include <Core/Engine.h>

Mesh* Objects::CreateTable(std::string name, float width, float height, float ballRadius, glm::vec3 marginColor, glm::vec3 coverColor, glm::vec3 woodColor)
{

	float length = 2 * width, marginSize = width / 10;
	std::vector<VertexFormat> vertices;

	vertices.push_back(VertexFormat(glm::vec3(length, height, width), coverColor));		//0
	vertices.push_back(VertexFormat(glm::vec3(-length, height, width), coverColor));	//1
	vertices.push_back(VertexFormat(glm::vec3(length, height, -width), coverColor));	//2
	vertices.push_back(VertexFormat(glm::vec3(-length, height, -width), coverColor));	//3

	vertices.push_back(VertexFormat(glm::vec3(length, height, width), marginColor));	//4
	vertices.push_back(VertexFormat(glm::vec3(-length, height, width), marginColor));	//5
	vertices.push_back(VertexFormat(glm::vec3(length, height, -width), marginColor));	//6
	vertices.push_back(VertexFormat(glm::vec3(-length, height, -width), marginColor));	//7

	height += ballRadius;
	vertices.push_back(VertexFormat(glm::vec3(length, height, width), marginColor));	//8
	vertices.push_back(VertexFormat(glm::vec3(-length, height, width), marginColor));	//9
	vertices.push_back(VertexFormat(glm::vec3(length, height, -width), marginColor));	//10
	vertices.push_back(VertexFormat(glm::vec3(-length, height, -width), marginColor));	//11

	length += marginSize;
	width += marginSize;
	vertices.push_back(VertexFormat(glm::vec3(length, height, width), marginColor));	//12
	vertices.push_back(VertexFormat(glm::vec3(-length, height, width), marginColor));	//13
	vertices.push_back(VertexFormat(glm::vec3(length, height, -width), marginColor));	//14
	vertices.push_back(VertexFormat(glm::vec3(-length, height, -width), marginColor));	//15

	vertices.push_back(VertexFormat(glm::vec3(length, height, width), woodColor));	//16
	vertices.push_back(VertexFormat(glm::vec3(-length, height, width), woodColor));	//17
	vertices.push_back(VertexFormat(glm::vec3(length, height, -width), woodColor));	//18
	vertices.push_back(VertexFormat(glm::vec3(-length, height, -width), woodColor));//19

	vertices.push_back(VertexFormat(glm::vec3(length, 0, width), woodColor));	//20
	vertices.push_back(VertexFormat(glm::vec3(-length, 0, width), woodColor));	//21
	vertices.push_back(VertexFormat(glm::vec3(length, 0, -width), woodColor));	//22
	vertices.push_back(VertexFormat(glm::vec3(-length, 0, -width), woodColor));	//23


	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 
		0, 2, 1, 2, 1, 3,
		6, 11, 7, 6, 10, 11,
		7, 9, 5, 7, 11, 9,
		5, 9, 8, 4, 5, 8,
		10, 6, 4, 4, 8, 10,
		15, 11, 10, 10, 14, 15,
		15, 13, 11, 11, 13, 9,
		9, 13, 12, 12, 8, 9,
		10, 8, 12, 12, 14, 10,
		16, 17, 20, 20, 17, 21,
		18, 16, 22, 22, 16, 20,
		19, 18, 23, 23, 18, 22,
		17, 19, 21, 21, 19, 23
		};

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}




glm::vec3 sphereCoordinates(int m, int n, int M, int N, float l) {
	float x, y, z;
	x = l * sin(3.14 * m / M) * cos(2 * 3.14 * n / N);
	y = l * sin(3.14 * m / M) * sin(2 * 3.14 * n / N);
	z = l * cos(3.14 * m / M);
	return glm::vec3(x, y, z);
}

Mesh* Objects::CreateBall(std::string name, glm::vec3 base, float radius, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	int M = 15, N = 15;
	for (int i = 0; i <= M; i++)
		for (int j = 0; j < N; j++) {
			vertices.push_back(VertexFormat(base + sphereCoordinates(i, j, M, N, radius), color));
		}
	M++; N++;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			indices.push_back(j + M * i);
			indices.push_back(j + M * (i + 1));
			indices.push_back(j + 1 + M * i);

			indices.push_back(j + M * (i + 1));
			indices.push_back(j + 1 + M * i);
			indices.push_back(j + 1 + M * (i + 1));
		}


	Mesh* circle = new Mesh(name);

	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh * Objects::CreateCue(std::string name, float length, float ball, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = {
		0, 1, 4, 4, 1, 5
	};

	vertices.push_back(VertexFormat(glm::vec3(ball, 0.01, 0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(ball, 0.01, -0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(ball, -0.01, 0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(ball, -0.01, -0.01), color));

	vertices.push_back(VertexFormat(glm::vec3(length, 0.01, 0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(length, 0.01, -0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(length, -0.01, 0.01), color));
	vertices.push_back(VertexFormat(glm::vec3(length, -0.01, -0.01), color));


	
	Mesh* cue = new Mesh(name);

	cue->InitFromData(vertices, indices);
	return cue;
}
