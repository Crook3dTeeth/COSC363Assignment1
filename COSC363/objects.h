#pragma once
#include <math.h> 
void loadObjects();
void amesWindow();
void loadAmesFile(const char* fname);
void moire();
void loadMoireFile(const char* fname);
void cradle();
void cradleBalls();
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
	float a = 0;
	float m = 1;
	float g = -9.81;
	float stringLength = 1.7;

	ball(float x, float y, float z, float offsetX, float offsetY)
	{
		ballX = x;
		ballY = y;
		ballZ = z;
		ballM = 0;
		ballAngle = 0;
		ballOffsetX = offsetX;
		ballOffsetY = offsetY;
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
};
class cradleBalls;