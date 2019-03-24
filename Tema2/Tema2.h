#pragma once
#include <Component/SimpleScene.h>
#include "Transform.h"
#include "Camera.h"


class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		bool BallInPocket(int ball);
		bool ballCollisions(int i, int j, float dt);
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, float offset);
		void BallReset(int index);
		bool StaticCollision(float x1, float z1, float x2, float z2, float r1, float r2);
		int NextPlayer();

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		CameraTema::Camera *camera;
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		float ballRadius, tableHeight, tableWidth, cueAngle, cueCharge, pocketRadius;
		bool aiming, chargingPos, chargingNeg, breakPosition, ballsStationary, faultPosition, gameEnded, scored;
		char playerColors[2][10];
		int currentPlayer, redBallsLeft, yellowBallsLeft, contact, numberOfFaults[2];

		float x[16], z[16], dx[16], dz[16];
		bool ballActive[16];
		char ballColor[16][10];
		float presetRedsX[7], presetRedsZ[7], presetYellowX[7], presetYellowZ[7];

};
