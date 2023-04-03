#pragma once
#include <iostream>
#include <fstream>
#include <climits>
#include <list>
#include <vector>
#include <cstdlib>
#include <math.h> 
#include <GL/freeglut.h>
using namespace std;


void loadObjects();
void amesWindow();
void loadAmesFile(const char* fname);
void moire();
void loadMoireFile(const char* fname);
void cradle();
void axis(float lineWidth = 4.0f);


class ball {
public:
	float ballX;
	float ballY;
	float ballZ;
	float ballM;
	float ballAngle;
	float ballOffsetX;
	float ballOffsetY;
	float energyLoss = 0.98;
	float a = 0;
	float m = 1;
	float g = -9.81;
	float stringLength = 1.7;
	float cradleRadius = 0.15;

	// Shadow stuff
	float light[4];
	float shadowMat[16];

	ball(float x, float y, float z, float offsetX, float offsetY, float ligPos[4], float shad[16])
	{
		ballX = x;
		ballY = y;
		ballZ = z;
		ballM = 0;
		ballAngle = 0;
		ballOffsetX = offsetX;
		ballOffsetY = offsetY;
		copy(ligPos, ligPos + 4, light);
		copy(shad, shad + 16, shadowMat);
	}

	void update()
	{
		ballX = (ballOffsetX)+stringLength * sin((ballAngle * 3.1415 / 180));
		ballY = (ballOffsetY)+stringLength * -cos((ballAngle * 3.1415 / 180));
	}

	void updatePosition(float dt)
	{
		a = -(-stringLength * m * g * cos((ballAngle - 45) * 3.1415 / 180) - stringLength * m * g * sin((ballAngle - 45) * 3.1415 / 180));

		// p = mv , m = 1, p = v
		// vf = vi + a*t
		ballM = ballM + a * dt;

		ballAngle += ballM * dt + 0.5 * a * dt * dt;

		ballX = (ballOffsetX)+stringLength * sin((ballAngle * 3.1415 / 180));
		ballY = (ballOffsetY)+stringLength * -cos((ballAngle * 3.1415 / 180));
	}

	void resetBall()
	{
		ballAngle = 0;
		ballM = 0;
		update();
	}

	void renderBall()
	{
		glLightfv(GL_LIGHT3, GL_POSITION, light);
		glEnable(GL_LIGHTING);
		//cout << ballM << endl;
		glPushMatrix();
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(ballX, ballY, ballZ);
		glutSolidSphere(cradleRadius, 30, 10);
		glPopMatrix();


		// Ball's shadow
		glDisable(GL_LIGHTING);
		glColor4f(0.3, 0.3, 0.3, 0.1); 
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glPushMatrix();
			glMultMatrixf(shadowMat);
			glTranslatef(ballX, ballY, ballZ);
			glutSolidSphere(cradleRadius, 20, 30);
		glPopMatrix();
		glEnable(GL_LIGHTING);
		//glDisable(GL_BLEND);
		
	}
};

class cradleBalls {
public:
	vector<ball> cradleData;
	float cradleHeight;
	float cradleStandX;
	float cradleStandY;
	float cradleStandZ;

	cradleBalls(float cradleX, float cradleY, float cradleZ, float cHeight = 0.7f)
	{
		cradleHeight = cHeight;
		cradleStandX = cradleX;
		cradleStandY = cradleY;
		cradleStandZ = cradleZ;
	}

	void inset(ball data)
	{
		cradleData.push_back(data);
	}

	void updateBalls(float dt)
	{
		for (int i = 0; i < cradleData.size(); i++) {
			cradleData[i].updatePosition(dt);
		}
	}

	void print()
	{

		cout << cradleData.size() << endl;
		
	}

	// Draws the balls and the strings
	void renderBalls()
	{
		for (int i = 0; i < cradleData.size(); i++) {
			cradleData[i].renderBall();
		}
	}

};