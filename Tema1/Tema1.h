#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>



class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	int rows = 8, columns = 13;
	int brickTotals;
	float timeSpent, winAnimation = 0;
	int brickLives[8][13], powerUps[8][13];
	float brickposX[8][13], brickposY[8][13], brickScale[8][13];
	bool powerActive[8][13];
	float powerposX[8][13], powerposY[8][13], fallSpeed = 300, rotateAngle[8][13];
	float timerPower[3];

	glm::mat3 modelMatrix;
	int triesLeft;
	char cheatState = 0;
	
	bool ballActive = false, sticky = false;
	float padWidth = 200, padThickness = 10, padX;
	float ballX, ballY, speedX, speedY , ballRadius, ballSpeed = 625, nextX, nextY;
	float brickX, brickY;
	float wallThickness = 40, deltaSticky;
	int launchX = 0, launchY;
	float frameColor = 0.05;
};
