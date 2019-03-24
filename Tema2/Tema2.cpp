#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Objects.h"

#define WHITE 15

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

bool Tema2::StaticCollision(float x1, float z1, float x2, float z2, float r1, float r2) {

	float sqDist = (x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2);
	float sqRad = (r1 + r2) * (r1 + r2);

	return (sqDist < sqRad);
}

void Tema2::BallReset(int ball) {
	ballActive[ball] = true;
	dx[ball] = 0;
	dz[ball] = 0;

	if (strstr(ballColor[ball], "red")) {
		for (int poz = 0; poz < 7; poz++) {
			x[ball] = presetRedsX[poz];
			z[ball] = presetRedsZ[poz];
			bool wrongPosition = false;
			for (int i = 0; i < 16; i++)
				if (ball != i)
					if (StaticCollision(x[ball], z[ball], x[i], z[i], ballRadius, ballRadius))
						wrongPosition = true;
			if (wrongPosition == false)
				return;
		}
	}

	if (strstr(ballColor[ball], "yellow")) {
		for (int poz = 0; poz < 7; poz++) {
			x[ball] = presetYellowX[poz];
			z[ball] = presetYellowZ[poz];
			bool wrongPosition = false;
			for (int i = 0; i < 16; i++)
				if (ball != i)
					if (StaticCollision(x[ball], z[ball], x[i], z[i], ballRadius, ballRadius))
						wrongPosition = true;
			if (wrongPosition == false)
				return;
		}
	}

	if (strstr(ballColor[ball], "black")) {
		float sqrt3 = sqrt(3);
		x[ball] = tableWidth + (ballRadius + 0.001) * sqrt3 * 2 + ballRadius;
		z[ball] = 0;
		bool wrongPosition = true;
		while (wrongPosition) {
			x[ball] -= ballRadius;
			wrongPosition = false;
			for (int i = 0; i < 16; i++)
				if (i != ball)
					if (StaticCollision(x[ball], z[ball], x[i], z[i], ballRadius, ballRadius))
						wrongPosition = true;
		}
		return;
	}

	// white
	x[ball] = -tableWidth;
	z[ball] = 0;
	bool wrongPosition = true;
	while (wrongPosition) {
		wrongPosition = false;
		for (int i = 0; i < 16; i++) {
			if (i != ball)
				if (StaticCollision(x[ball], z[ball], x[i], z[i], ballRadius, ballRadius))
					wrongPosition = true;
		}
		x[ball] -= ballRadius;
	}

}


void Tema2::Init()
{

	{
		camera = new CameraTema::Camera();
		camera->specView();
		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

		ballRadius = 0.1;
		tableWidth = 2;
		tableHeight = 1;
		pocketRadius = 0.15;
		currentPlayer = 0;
		redBallsLeft = 7;
		yellowBallsLeft = 7;
		aiming = false;
		chargingPos = false;
		chargingNeg = false;
		contact = -1;
		breakPosition = true;
		ballsStationary = true;
		faultPosition = false;
		scored = false;
		gameEnded = false;
		strcpy(playerColors[0], "N/A");
		strcpy(playerColors[1], "N/A");
		numberOfFaults[0] = 0;
		numberOfFaults[1] = 0;

		cueCharge = 0;
	}

	// initialize ball variables
	{
		strcpy(ballColor[0], "red");
		strcpy(ballColor[1], "red");
		strcpy(ballColor[2], "yellow");
		strcpy(ballColor[3], "yellow");
		strcpy(ballColor[4], "black");
		strcpy(ballColor[5], "red");
		strcpy(ballColor[6], "yellow");
		strcpy(ballColor[7], "red");
		strcpy(ballColor[8], "yellow");
		strcpy(ballColor[9], "yellow");
		strcpy(ballColor[10], "red");
		strcpy(ballColor[11], "red");
		strcpy(ballColor[12], "yellow");
		strcpy(ballColor[13], "yellow");
		strcpy(ballColor[14], "red");
		strcpy(ballColor[15], "white");

		float sqrt3 = sqrt(3);
		presetRedsX[0] = tableWidth;
		presetRedsZ[0] = 0;
		presetRedsX[1] = tableWidth + (ballRadius + 0.001) * sqrt3;
		presetRedsZ[1] = -(ballRadius + 0.001);
		presetRedsX[2] = tableWidth + (ballRadius + 0.001) * sqrt3 * 2;
		presetRedsZ[2] = 2 * (ballRadius + 0.001);
		presetRedsX[3] = tableWidth + (ballRadius + 0.001) * sqrt3 * 3;
		presetRedsZ[3] = (ballRadius + 0.001);
		presetRedsX[4] = tableWidth + (ballRadius + 0.001) * sqrt3 * 3;
		presetRedsZ[4] = -3 * (ballRadius + 0.001);
		presetRedsX[5] = tableWidth + (ballRadius + 0.001) * sqrt3 * 4;
		presetRedsZ[5] = 4 * (ballRadius + 0.001);
		presetRedsX[6] = tableWidth + (ballRadius + 0.001) * sqrt3 * 4;
		presetRedsZ[6] = -2 * (ballRadius + 0.001);



		presetYellowX[0] = tableWidth + (ballRadius + 0.001) * sqrt3;
		presetYellowX[1] = tableWidth + (ballRadius + 0.001) * sqrt3 * 2;
		presetYellowX[2] = tableWidth + (ballRadius + 0.001) * sqrt3 * 3;
		presetYellowX[3] = tableWidth + (ballRadius + 0.001) * sqrt3 * 3;
		presetYellowX[4] = tableWidth + (ballRadius + 0.001) * sqrt3 * 4;
		presetYellowX[5] = tableWidth + (ballRadius + 0.001) * sqrt3 * 4;
		presetYellowX[6] = tableWidth + (ballRadius + 0.001) * sqrt3 * 4;

		presetYellowZ[0] = (ballRadius + 0.001);
		presetYellowZ[1] = -2 * (ballRadius + 0.001);
		presetYellowZ[2] = 3 * (ballRadius + 0.001);
		presetYellowZ[3] = -(ballRadius + 0.001);
		presetYellowZ[4] = 2 * (ballRadius + 0.001);
		presetYellowZ[5] = 0;
		presetYellowZ[6] = -4 * (ballRadius + 0.001);


		for (int i = 0; i < 16; i++) {
			BallReset(i);
		}
	}

	//meshes
	{
		Mesh* table = Objects::CreateTable("table", tableWidth, tableHeight, ballRadius, glm::vec3(0.3, 0.5, 0), glm::vec3(0.1, 0.7, 0.1), glm::vec3(0.2, 0.2, 0));
		AddMeshToList(table);

		Mesh* blackBall = Objects::CreateBall("blackBall", glm::vec3(0, 0, 0), ballRadius, glm::vec3(0.05, 0.05, 0.05));
		AddMeshToList(blackBall);
		Mesh* redBall = Objects::CreateBall("redBall", glm::vec3(0, 0, 0), ballRadius, glm::vec3(0.8, 0.05, 0.05));
		AddMeshToList(redBall);
		Mesh* yellowBall = Objects::CreateBall("yellowBall", glm::vec3(0, 0, 0), ballRadius, glm::vec3(0.8, 0.8, 0));
		AddMeshToList(yellowBall);
		Mesh* whiteBall = Objects::CreateBall("whiteBall", glm::vec3(0, 0, 0), ballRadius, glm::vec3(0.95, 0.95, 0.95));
		AddMeshToList(whiteBall);

		Mesh* cue1 = Objects::CreateCue("cue1", tableWidth, ballRadius, glm::vec3(0.1, 0, 0.25));
		AddMeshToList(cue1);
		Mesh* cue2 = Objects::CreateCue("cue2", tableWidth, ballRadius, glm::vec3(0.25, 0.15, 0));
		AddMeshToList(cue2);

		Mesh* pocket = Objects::CreateBall("pocket", glm::vec3(0, 0, 0), pocketRadius, glm::vec3(0, 0.05, 0.05));
		AddMeshToList(pocket);
		pocket = Objects::CreateBall("pocket2", glm::vec3(0, 0, 0), pocketRadius + ballRadius, glm::vec3(0, 0.05, 0.05));
		AddMeshToList(pocket);
	}

	//shader
	{
		Shader *shader = new Shader("CueShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}


}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{

	// render table
	RenderMesh(meshes["table"], shaders["VertexColor"], glm::mat4(1));

	// render pockets
	{
		modelMatrix = Transform::Translate(0, tableHeight, -tableWidth);
		RenderMesh(meshes["pocket"], shaders["VertexColor"], modelMatrix);
		modelMatrix = Transform::Translate(0, tableHeight, tableWidth);
		RenderMesh(meshes["pocket"], shaders["VertexColor"], modelMatrix);
		modelMatrix = Transform::Translate(-2 * tableWidth, tableHeight, -tableWidth);
		RenderMesh(meshes["pocket2"], shaders["VertexColor"], modelMatrix);
		modelMatrix = Transform::Translate(-2 * tableWidth, tableHeight, tableWidth);
		RenderMesh(meshes["pocket2"], shaders["VertexColor"], modelMatrix);
		modelMatrix = Transform::Translate(2 * tableWidth, tableHeight, -tableWidth);
		RenderMesh(meshes["pocket2"], shaders["VertexColor"], modelMatrix);
		modelMatrix = Transform::Translate(2 * tableWidth, tableHeight, tableWidth);
		RenderMesh(meshes["pocket2"], shaders["VertexColor"], modelMatrix);
	}

	// render balls
	for (int i = 0; i < 16; i++) {
		if (ballActive[i]) {
			modelMatrix = Transform::Translate(x[i], tableHeight + ballRadius, z[i]);
			if (strstr(ballColor[i], "red")) {
				RenderMesh(meshes["redBall"], shaders["VertexColor"], modelMatrix);
				continue;
			}
			if (strstr(ballColor[i], "yell")) {
				RenderMesh(meshes["yellowBall"], shaders["VertexColor"], modelMatrix);
				continue;
			}
			if (strstr(ballColor[i], "white")) {
				RenderMesh(meshes["whiteBall"], shaders["VertexColor"], modelMatrix);
				continue;
			}
			RenderMesh(meshes["blackBall"], shaders["VertexColor"], modelMatrix);
		}
	}

	// render cue
	if (chargingPos || chargingNeg) {
		if (chargingPos)
			cueCharge += deltaTimeSeconds / 2;
		else cueCharge -= deltaTimeSeconds / 2;
		if (cueCharge < 0) {
			chargingPos = true;
			chargingNeg = false;
		}
		if (cueCharge > 0.75) {
			chargingPos = false;
			chargingNeg = true;
		}
		modelMatrix = Transform::Translate(x[WHITE], tableHeight + ballRadius * 3 / 4, z[WHITE]);
		modelMatrix *= Transform::RotateOY(cueAngle);
		modelMatrix *= Transform::RotateOZ(RADIANS(20));
		if (currentPlayer)
			RenderMesh(meshes["cue1"], shaders["CueShader"], modelMatrix, cueCharge);
		else RenderMesh(meshes["cue2"], shaders["CueShader"], modelMatrix, cueCharge);
	}
	if (aiming) {
		modelMatrix = Transform::Translate(x[WHITE], tableHeight + ballRadius * 3 / 4, z[WHITE]);
		modelMatrix *= Transform::RotateOY(cueAngle);
		modelMatrix *= Transform::RotateOZ(RADIANS(20));
		if (currentPlayer)
			RenderMesh(meshes["cue1"], shaders["VertexColor"], modelMatrix);
		else
			RenderMesh(meshes["cue2"], shaders["VertexColor"], modelMatrix);
	}

	// change balls positions
	for (int i = 0; i <= WHITE; i++) {
		if (ballActive[i]) {
			x[i] += dx[i] * deltaTimeSeconds;
			z[i] += dz[i] * deltaTimeSeconds;
		}
	}

	// wall collisions
	for (int i = 0; i <= WHITE; i++) {
		if (ballActive[i]) {
			if (x[i] > 2 * tableWidth - ballRadius) {
				glm::vec3 v = glm::normalize(glm::vec3(-dx[i], 0, -dz[i]));
				float f = abs(x[i] - (2 * tableWidth - ballRadius));
				x[i] += v.x * f;
				z[i] += v.z * f;
				dx[i] = -dx[i];
			}
			else
				if (x[i] < -2 * tableWidth + ballRadius) {
					glm::vec3 v = glm::normalize(glm::vec3(-dx[i], 0, -dz[i]));
					float f = abs(x[i] - (-2 * tableWidth + ballRadius));
					x[i] += v.x * f;
					z[i] += v.z * f;
					dx[i] = -dx[i];
				}

			if (z[i] > tableWidth - ballRadius) {
				glm::vec3 v = glm::normalize(glm::vec3(-dx[i], 0, -dz[i]));
				float f = abs(z[i] - (tableWidth - ballRadius));
				x[i] += v.x * f;
				z[i] += v.z * f;
				dz[i] = -dz[i];
			}
			else
				if (z[i] < -tableWidth + ballRadius) {
					glm::vec3 v = glm::normalize(glm::vec3(-dx[i], 0, -dz[i]));
					float f = abs(z[i] - (-tableWidth + ballRadius));
					x[i] += v.x * f;
					z[i] += v.z * f;
					dz[i] = -dz[i];
				}
		}
	}

	// decelerations
	for (int i = 0; i <= WHITE; i++) {
		if (ballActive[i]) {
			if (dx[i] && dz[i]) {
				float decelX = dx[i] / (abs(dx[i]) + abs(dz[i])) * deltaTimeSeconds * 0.7;
				float decelZ = dz[i] / (abs(dx[i]) + abs(dz[i])) * deltaTimeSeconds * 0.7;
				if (dx[i] * (dx[i] - decelX) < 0)
					dx[i] = 0;
				else dx[i] -= decelX;
				if (dz[i] * (dz[i] - decelZ) < 0)
					dz[i] = 0;
				else dz[i] -= decelZ;
			}
		}
	}

	// ball collision detection
	for (int i = 0; i < WHITE; i++) {
		for (int j = i + 1; j <= WHITE; j++) {
			if (ballActive[i] && ballActive[j])
				if (ballCollisions(i, j, deltaTimeSeconds)){
					if (contact < 0)
						contact = i;
				}
		}
	}

	// pocket detection
	for (int i = 0; i < WHITE; i++) {
		if (ballActive[i]) {
			if (BallInPocket(i)) {
				// colors not assigned yet
				if (strstr(playerColors[currentPlayer], "N/A")) {
					if (strstr(ballColor[i], "red")) {
						strcpy(playerColors[currentPlayer], "red");
						strcpy(playerColors[NextPlayer()], "yellow");
						printf("Colors are now assigned. Player %d red | Player %d yellow\n", currentPlayer, NextPlayer());
						redBallsLeft--;
					}
					if (strstr(ballColor[i], "yellow")) {
						strcpy(playerColors[currentPlayer], "yellow");
						strcpy(playerColors[NextPlayer()], "red");
						printf("Colors are now assigned. Player %d yellow | Player %d red\n", currentPlayer, NextPlayer());
						yellowBallsLeft--;
					}
					if (strstr(ballColor[i], "black")) {
						gameEnded = true;
						printf("Player %d lost the game by pocketing the black.\n", currentPlayer);
					}
					contact = i;
					scored = true;
				}
				else {
					if (strstr(ballColor[i], "black")) {
						gameEnded = true;
						if (strstr(playerColors[currentPlayer], "red"))
							if (redBallsLeft > 0)
								printf("Player %d lost the game by pocketing the black.\n", currentPlayer);
							else printf("Player %d WON the game.", currentPlayer);
						else
							if (yellowBallsLeft > 0)
								printf("Player %d lost the game by pocketing the black.\n", currentPlayer);
							else printf("Player %d WON the game.", currentPlayer);
					}
					if (strstr(ballColor[i], "red")) {
						redBallsLeft--;
						if (strstr(playerColors[currentPlayer], "red"))
							scored = true;
					}
					if (strstr(ballColor[i], "yellow")) {
						yellowBallsLeft--;
						if (strstr(playerColors[currentPlayer], "yellow"))
							scored = true;
					}
				}
			}
		}
	}

	//white ball pocket detection
	if (BallInPocket(WHITE))
		contact = -2;

	// movement finish?
	if (!ballsStationary) {
		bool stillMoving = false;
		for (int i = 0; i <= WHITE; i++) {
			if (ballActive[i])
				if (dx[i] != 0 || dz[i] != 0) {
					stillMoving = true;
					break;
				}
		}
		if (!stillMoving) {
			ballsStationary = true;

			int next = currentPlayer;
			if (scored == false)
				next = NextPlayer();
			if (contact < 0) {
				next = NextPlayer();
				ballActive[WHITE] = false;
				numberOfFaults[currentPlayer]++;
			}
			else
				if (!strstr(playerColors[currentPlayer], ballColor[contact])) {
					if ((strstr(ballColor[contact], "black") && redBallsLeft * yellowBallsLeft == 0) || strstr(playerColors[0], "N/A"))
						next = next;
					else {
						ballActive[WHITE] = false;
						next = NextPlayer();
						numberOfFaults[currentPlayer]++;
					}
				}
			scored = false;
			contact = -1;
			currentPlayer = next;
			printf("Player %d's turn. Color: %s\n", currentPlayer, playerColors[currentPlayer]);
		}
	}
	

	// reset white ball
	if (!ballActive[WHITE] && ballsStationary) {
		faultPosition = true;
		BallReset(WHITE);
		//currentPlayer = NextPlayer();
	}



}

bool Tema2::BallInPocket(int ball) {
	
	float bigDist = ballRadius + pocketRadius + ballRadius;
	float smallDist = ballRadius + pocketRadius;
	bigDist = bigDist * bigDist;
	smallDist = smallDist * smallDist;

	if (smallDist > x[ball] * x[ball] + (abs(z[ball]) - tableWidth) * (abs(z[ball]) - tableWidth)) {
		ballActive[ball] = false;
		return true;
	}

	if (bigDist > (abs(x[ball]) - 2 * tableWidth) * (abs(x[ball]) - 2 * tableWidth) + (abs(z[ball]) - tableWidth) * (abs(z[ball]) - tableWidth)) {
		ballActive[ball] = false;
		return true;
	}
	return false;
}

bool Tema2::ballCollisions(int i, int j, float dt) {
	float ax = x[i], bx = x[j], vax = dx[i] / dt, vbx = dx[j] / dt;
	float az = z[i], bz = z[j], vaz = dz[i] / dt, vbz = dz[j] / dt;
	double distance = sqrt((ax - bx) * (ax - bx) + (az - bz) * (az - bz));
	distance -= 2 * ballRadius;

	float movement = sqrt((vax - vbx) * (vax - vbx) + (vaz - vbz) * (vaz - vbz));
	if (movement < distance)
		return false;

	glm::vec3 n = glm::vec3(vax - vbx, 0, vaz - vbz);
	n = normalize(n);
	glm::vec3 c = glm::vec3(bx - ax, 0, bz - az);

	double d = dot(n, c);
	if (d <= 0)
		return false;

	double lengthC = sqrt(c.x * c.x + c.z * c.z);
	double f = lengthC * lengthC - d * d;

	if (f >= (ballRadius + ballRadius) * (ballRadius + ballRadius))
		return false;

	double t = (ballRadius + ballRadius) * (ballRadius + ballRadius) - f;
	if (t < 0)
		return false;

	double distOnVector = d - sqrt(t);
	if (distOnVector >= distance)
		return false;

	float part;
	if (distance > 0)
		part = distOnVector / distance;
	else part = 0;

	x[i] += dx[i] * dt * part;
	x[j] += dx[j] * dt * part;
	z[i] += dz[i] * dt * part;
	z[j] += dz[j] * dt * part;

	// reflecting

	n = glm::vec3(ax - bx, 0, az - bz);
	n = normalize(n);
	
	glm::vec3 v1 = glm::vec3(dx[i], 0, dz[i]);
	glm::vec3 v2 = glm::vec3(dx[j], 0, dz[j]);
	float a1 = dot(n, v1);
	float a2 = dot(n, v2);
	float p = a1 - a2;

	glm::vec3 new1 = v1 - p * n;
	glm::vec3 new2 = v2 + p * n;

	dx[i] = new1.x;
	dz[i] = new1.z;
	dx[j] = new2.x;
	dz[j] = new2.z;
	

	if (i == WHITE || j == WHITE)
		return true;
	return false;
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (ballsStationary) {
		if (breakPosition) {
			if (window->KeyHold(GLFW_KEY_W)) {
				z[WHITE] -= deltaTime;
				if (z[WHITE] < -tableWidth + ballRadius)
					z[WHITE] = ballRadius - tableWidth;
			}
			if (window->KeyHold(GLFW_KEY_S)) {
				z[WHITE] += deltaTime;
				if (z[WHITE] > tableWidth - ballRadius)
					z[WHITE] = -ballRadius + tableWidth;
			}
			if (window->KeyHold(GLFW_KEY_A)) {
				x[WHITE] -= deltaTime;
				if (x[WHITE] < ballRadius - tableWidth * 2)
					x[WHITE] = ballRadius - tableWidth * 2;
			}
			if (window->KeyHold(GLFW_KEY_D)) {
				x[WHITE] += deltaTime;
				if (x[WHITE] > -tableWidth)
					x[WHITE] = -tableWidth;
			}
		}

		if (faultPosition) {
			if (window->KeyHold(GLFW_KEY_W)) {
				z[WHITE] -= deltaTime;
				if (z[WHITE] < -tableWidth + ballRadius)
					z[WHITE] = ballRadius - tableWidth;
			}
			if (window->KeyHold(GLFW_KEY_S)) {
				z[WHITE] += deltaTime;
				if (z[WHITE] > tableWidth - ballRadius)
					z[WHITE] = -ballRadius + tableWidth;
			}
			if (window->KeyHold(GLFW_KEY_A)) {
				x[WHITE] -= deltaTime;
				if (x[WHITE] < ballRadius - tableWidth * 2)
					x[WHITE] = ballRadius - tableWidth * 2;
			}
			if (window->KeyHold(GLFW_KEY_D)) {
				x[WHITE] += deltaTime;
				if (x[WHITE] > tableWidth * 2 - ballRadius)
					x[WHITE] = tableWidth * 2 - ballRadius;
			}
		}
	}
	

}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && ballsStationary)
	{
		if (gameEnded) {
			int p0balls, p1balls;
			if (strstr(playerColors[0], "red")) {
				p0balls = redBallsLeft;
				p1balls = yellowBallsLeft;
			}
			printf("=======================\n");
			printf("==========  p0  |  p1\n");
			printf("faults ==== %d     %d\n", numberOfFaults[0], numberOfFaults[1]);
			printf("balls left  %d     %d\n", p0balls, p1balls);
		}
		else {
			breakPosition = false;
			faultPosition = false;
			aiming = true;
			camera->aimView(x[WHITE], tableHeight + ballRadius, z[WHITE]);

			cueAngle = atan2(z[WHITE], x[WHITE]);
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (aiming && window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		float sensitivity = -0.001;

		cueAngle -= sensitivity * deltaX;
		camera->RotateThirdPerson_OY(sensitivity * deltaX);
	}

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == 1 && aiming) {
		aiming = false;
		chargingPos = true;
		chargingNeg = false;
	}
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (button == 1 && (chargingPos || chargingNeg)) {
		chargingPos = false;
		chargingNeg = false;
		ballsStationary = false;
		//TODO viteza bilei
		glm::vec3 direction = camera->forward;
		dx[WHITE] = direction.x * cueCharge * 10;
		dz[WHITE] = direction.z * cueCharge * 10;

		cueCharge = 0;
		camera->specView();
	}
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

int Tema2::NextPlayer() {
	return (currentPlayer + 1) % 2;
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, float offset)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int t = glGetUniformLocation(shader->program, "Time");
	glUniform1f(t, offset);

	mesh->Render();
}

