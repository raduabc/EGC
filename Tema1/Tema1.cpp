#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// compute coordinates of square center
	// float cx = corner.x + squareSide / 2;
	// float cy = corner.y + squareSide / 2;

	glm::vec3 brickColor = glm::vec3(0.3, 0, 0);
	glm::vec3 wallColor = glm::vec3(0.8, 0.4, 0.1);
	glm::vec3 padColor = glm::vec3(0.1, 0.7, 0.4);
	
	timeSpent = 0;
	brickTotals = rows * columns;

	brickY = (resolution.y / 3) / rows * 2 / 3;
	brickX = (resolution.x * 4 / 5) / columns * 2 / 3;
	ballRadius = brickX / 5;
	triesLeft = 3;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++) {
			brickposX[i][j] = resolution.x / 10 + j * brickX * 3 / 2 + brickX / 3;
			brickposY[i][j] = i * brickY * 3 / 2 + resolution.y / 2;
			brickLives[i][j] = 1;
			brickScale[i][j] = 1;
			powerposX[i][j] = brickposX[i][j] + brickX / 2 - brickY / 2;
			powerposY[i][j] = brickposY[i][j];
			powerActive[i][j] = true;
			rotateAngle[i][j] = 0;
			int choose = rand() % 100 + 1; // 1-100
			if (choose < 86) {
				powerActive[i][j] = false;
				continue;
			}
			if (choose < 90) {
				powerUps[i][j] = 0;
				continue;
			}
			if (choose < 93) {
				powerUps[i][j] = 1;
				continue;
			}
			if (choose < 96) {
				powerUps[i][j] = 2;
				continue;
			}
			if (choose < 98) {
				powerUps[i][j] = 3;
				continue;
			}
			else 
				powerUps[i][j] = 4;
		}
	timerPower[0] = 0;
	timerPower[1] = 0;
	timerPower[2] = 0;

	Mesh* fullBrick = Object2D::CreateRectangle("brick", corner, brickX, brickY, brickColor);
	AddMeshToList(fullBrick);
	Mesh* emptyBrick = Object2D::CreateRectangle("emptyBrick", corner, brickX, brickY, brickColor, false);
	AddMeshToList(emptyBrick);

	Mesh* pad = Object2D::CreateRectangle("pad", corner, padWidth, padThickness, padColor);
	AddMeshToList(pad);

	Mesh* wall = Object2D::CreateRectangle("wall", corner, wallThickness, max(resolution.x, resolution.y), wallColor, true);
	AddMeshToList(wall);

	Mesh* power1 = Object2D::CreateRectangle("power1", corner, brickY, brickY, wallColor);
	AddMeshToList(power1);
	Mesh* power0 = Object2D::CreateRectangle("power0", corner, brickY, brickY, glm::vec3(0.2, 1, 0));
	AddMeshToList(power0);
	Mesh* power2 = Object2D::CreateHeart("power2", corner, brickY / 2, glm::vec3(1, 0.4, 0.6));
	AddMeshToList(power2);
	Mesh* power3 = Object2D::CreateRectangle("power3", corner, brickY, brickY, brickColor);
	AddMeshToList(power3);
	Mesh* power4 = Object2D::CreateRectangle("power4", corner, brickY, brickY, glm::vec3(0.9, 0.9, 0));
	AddMeshToList(power4);

	Mesh* ball = Object2D::CreateCircle("ball", corner, ballRadius, glm::vec3(0.2, 0.8, 0.2));
	AddMeshToList(ball);

	Mesh* tries = Object2D::CreateCircle("tries", corner, wallThickness / 3, glm::vec3(0.1, 0.1, 0.3));
	AddMeshToList(tries);

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, frameColor, frameColor, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
	timeSpent += deltaTimeSeconds;

	//  tries left
	if (triesLeft > 0) {
	modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(wallThickness / 2, 100);
		RenderMesh2D(meshes["tries"], shaders["VertexColor"], modelMatrix);
		if (triesLeft > 1) {
			modelMatrix *= Transform2D::Translate(0, wallThickness);
			RenderMesh2D(meshes["tries"], shaders["VertexColor"], modelMatrix);
			if (triesLeft > 2) {
				modelMatrix *= Transform2D::Translate(0, wallThickness);
				RenderMesh2D(meshes["tries"], shaders["VertexColor"], modelMatrix);
			}
		}
	}

	// winner animation
	if (winAnimation) {
		if (triesLeft > 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate((resolution.x / 2 - wallThickness / 2 - 4 * brickY) * winAnimation / 5 + wallThickness / 2, (resolution.y / 2 - 100 - 2 * wallThickness) * winAnimation / 5 + 100 + 2 * wallThickness);
			modelMatrix *= Transform2D::Rotate(winAnimation * 780 / 5);
			modelMatrix *= Transform2D::Scale(winAnimation, winAnimation);
			RenderMesh2D(meshes["power2"], shaders["VertexColor"], modelMatrix);
		}
		if (triesLeft > 1) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate((resolution.x / 2 - wallThickness / 2) * winAnimation / 5 + wallThickness / 2, (resolution.y / 2 - 100 - wallThickness + 25) * winAnimation / 5 + 100 + wallThickness);
			modelMatrix *= Transform2D::Rotate(winAnimation * 720 / 5);
			modelMatrix *= Transform2D::Scale(winAnimation, winAnimation);
			RenderMesh2D(meshes["power2"], shaders["VertexColor"], modelMatrix);
		}
		if (triesLeft > 2) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate((resolution.x / 2 - wallThickness / 2 + 4 * brickY) * winAnimation / 5 + wallThickness / 2, (resolution.y / 2 - 100) * winAnimation / 5 + 100);
			modelMatrix *= Transform2D::Rotate(winAnimation * 660 / 5);
			modelMatrix *= Transform2D::Scale(winAnimation, winAnimation);
			RenderMesh2D(meshes["power2"], shaders["VertexColor"], modelMatrix);
		}
		if (winAnimation < 5)
			winAnimation += deltaTimeSeconds;
		else
			frameColor = 0.3;
	}

	// left wall
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 50);
	RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);
	// right wall
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(resolution.x - wallThickness, 50);
	RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);
	// top wall
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, resolution.y);
	modelMatrix *= Transform2D::Rotate(-90);
	RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);
	// pad
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(padX - padWidth / 2, wallThickness);
	RenderMesh2D(meshes["pad"], shaders["VertexColor"], modelMatrix);
	//  bricks
	for (int i = 0; i < rows; i ++) {
		for (int j = 0; j < columns; j++) {
			if (brickLives[i][j]) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(brickposX[i][j], brickposY[i][j]);
				RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
			}
			else {
				if (brickScale[i][j] > 0) { //  brick dissapears
					brickScale[i][j] -= deltaTimeSeconds;
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(brickposX[i][j] + brickX / 2, brickposY[i][j] + brickY / 2);
					modelMatrix *= Transform2D::Scale(brickScale[i][j], brickScale[i][j]);
					modelMatrix *= Transform2D::Translate(- brickX / 2, - brickY / 2);
					RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
				}
				else { // power-ups fall
					if (powerActive[i][j]) {
						modelMatrix = glm::mat3(1);
						powerposY[i][j] -= fallSpeed * deltaTimeSeconds;
						modelMatrix *= Transform2D::Translate(brickY / 2 + powerposX[i][j], brickY / 2 + powerposY[i][j]);
						modelMatrix *= Transform2D::Rotate(rotateAngle[i][j]);
						modelMatrix *= Transform2D::Translate(-brickY / 2, -brickY / 2);
						if (powerUps[i][j] == 1)
							RenderMesh2D(meshes["power1"], shaders["VertexColor"], modelMatrix);
						if (powerUps[i][j] == 2)
							RenderMesh2D(meshes["power2"], shaders["VertexColor"], modelMatrix);
						if (powerUps[i][j] == 3)
							RenderMesh2D(meshes["power3"], shaders["VertexColor"], modelMatrix);
						if (powerUps[i][j] == 4)
							RenderMesh2D(meshes["power4"], shaders["VertexColor"], modelMatrix);
						if (powerUps[i][j] == 0)
							RenderMesh2D(meshes["power0"], shaders["VertexColor"], modelMatrix);
						rotateAngle[i][j] += deltaTimeSeconds * 50;
					}
				}
			}
		}
	}

	//  ball
	if (ballActive) {
		ballY += speedY * deltaTimeSeconds;
		ballX += speedX * deltaTimeSeconds;
		nextY = ballY + speedY * deltaTimeSeconds;
		nextX = ballX + speedX * deltaTimeSeconds;
	}
	else {
		ballY = wallThickness + padThickness + ballRadius;
		ballX = padX + deltaSticky;
	}
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(ballX, ballY);
	RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);

	// Power-up timers
	timerPower[0] -= deltaTimeSeconds;
	timerPower[1] -= deltaTimeSeconds;
	timerPower[2] -= deltaTimeSeconds;

	if (timerPower[0] > 0) { // bottom wall
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, wallThickness - padThickness);
		modelMatrix *= Transform2D::Rotate(-90);
		RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);
	}

	if (timerPower[2] < 0)
		ballSpeed = 550;
	else ballSpeed = 900;


	if (ballActive) {
		//  wall collision
		if (nextY >= resolution.y - wallThickness - ballRadius) {
			speedY = -speedY;
		}
		// sides
		if ((nextX >= resolution.x - wallThickness - ballRadius || nextX <= wallThickness + ballRadius) && nextY >= wallThickness) {

			speedX = -speedX;
		}
		// power-up wall
		if (nextY - ballRadius < wallThickness - padThickness && timerPower[0] > 0) {
			speedY = -speedY;
			timerPower[0] = 0;
		}
		//  pad collision
		if (ballY <= wallThickness + padThickness + ballRadius) {
			if (ballX < padX + padWidth / 2 && ballX > padX - padWidth / 2 && ballY > wallThickness) {
				if (sticky) {
					ballActive = false;
					deltaSticky = ballX - padX;
				}
				else {
					float distance = ballX - padX;
					distance = distance / (padWidth / 2);
					float angle = acos(distance);
					speedY = abs(sin(angle) * ballSpeed);
					speedX = cos(angle) * ballSpeed;
					//printf("speedY %f | speedX %f | speed %f\n", speedY, speedX, sqrt(speedX*speedX + speedY * speedY));
				}
			}
		}
		//  death
		if (ballY < ballRadius) {
			deltaSticky = 0;
			triesLeft--;
			sticky = false;
			if (triesLeft >= 0)
				ballActive = false;
			else frameColor = 0;
		}

		//  brick collision
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (brickLives[i][j]) { // caramida intreaga
					if (ballX + ballRadius >= brickposX[i][j] &&
					ballX - ballRadius <= brickposX[i][j] + brickX &&
					ballY + ballRadius>= brickposY[i][j] &&
					ballY - ballRadius <= brickposY[i][j] + brickY) { // contact cu caramida
						brickLives[i][j]--;
						brickTotals--;
						if (timerPower[1] <= 0) {
							if (ballX >= brickposX[i][j] && ballX < brickposX[i][j] + brickX) // lovitura directa sus/jos
							{
								speedY = -speedY;
								continue;
							}
							if (ballY >= brickposY[i][j] && ballY < brickposY[i][j] + brickY) // stanga/dreapta
							{
								speedX = -speedX;
								continue;
							}
							if (ballX < brickposX[i][j]) { // colturile din stanga
								float aux = abs(speedX);
								speedX = -abs(speedY);
								if (ballY < brickposY[i][j]) // jos
									speedY = -aux;
								else
									speedY = aux; // sus
								continue;
							}
							else { // colturile din dreapta
								float aux = abs(speedX);
								speedX = abs(speedY);
								if (ballY < brickposY[i][j]) // jos
									speedY = -aux;
								else
									speedY = aux; // sus
								continue;
							}
						}
					}
				}
				else { // caramida moarta
					if (powerActive[i][j]) {
						if (powerposY[i][j] + brickY / 2 < 0) // pierdut
							powerActive[i][j] = false;
						if (powerposY[i][j] - brickY / 2 < padThickness + wallThickness && powerposY[i][j] + brickY / 2 > wallThickness) 
							if (powerposX[i][j] > padX - padWidth / 2 && powerposX[i][j] < padX + padWidth) {
								powerActive[i][j] = false;
								// tratez super-puterile
								if (powerUps[i][j] == 1) // zid
									if (timerPower[0] < 10)
										timerPower[0] = 10;
								if (powerUps[i][j] == 2) // vieti
									if (triesLeft == 0)
										triesLeft++;
								if (powerUps[i][j] == 3) // bila puternica
									if (timerPower[1] < 10)
										timerPower[1] = 10;
								if (powerUps[i][j] == 4) // pad puternic
									if (timerPower[2] < 5)
										timerPower[2] = 5;
								if (powerUps[i][j] == 0) { //sticky
									sticky = true;
								}
							}
					}
				}
			}
		}
	}

	// ending scenario
	if (brickTotals == 0) {
		speedX = 0;
		speedY = 0;
		if (winAnimation == 0)
			winAnimation += deltaTimeSeconds;
	}

}
void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_R) {
		ballActive = false;
		brickTotals = rows * columns;
		triesLeft = 3;
		sticky = false;
		for (int i = 0; i < 8; i ++)
			for (int j = 0; j < 13; j++) {
				brickLives[i][j] = 1;
				brickScale[i][j] = 1;
			}
	}
	// INVINCIBlE
	if (key == GLFW_KEY_I) {
		cheatState = 'i';
	}
	if (key == GLFW_KEY_N) {
		if (cheatState == 'i')
			cheatState = 'n';
		else cheatState = 0;
	}
	if (key == GLFW_KEY_V) {
		if (cheatState == 'n')
			cheatState = 'v';
		else cheatState = 0;
	}
	if (key == GLFW_KEY_C) {
		if (cheatState == 'n')
			cheatState = 'c';
		else cheatState = 0;
	}
	if (key == GLFW_KEY_B) {
		if (cheatState == 'i')
			cheatState = 'b';
		else cheatState = 0;
	}
	if (key == GLFW_KEY_L) {
		if (cheatState == 'b')
			cheatState = 'l';
		else cheatState = 0;
	}
	if (key == GLFW_KEY_E) {
		if (cheatState == 'l') {
			timerPower[0] = 60;
			timerPower[1] = 60;
			timerPower[2] = 60;
			triesLeft = 3;
			cheatState = 'e';
		}
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{

}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (launchX == 0)
		padX = mouseX;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && sticky && ballActive == false) {
		launchX = mouseX;
		launchY = mouseY;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && ballActive == false) {
		if (sticky) {
			sticky = false;
			int difX = launchX - mouseX;
			int difY = -launchY + mouseY;
			if (difY < 0)
				difX = -difX;
			float angle = atan2(difY, difX);
			speedY = abs(sin(angle)) * ballSpeed;
			speedX = cos(angle) * ballSpeed;
			ballActive = true;
			launchX = 0;
		}
		else {
			ballActive = true;
			speedX = 0;
			speedY = ballSpeed;
		}
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
