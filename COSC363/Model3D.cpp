//  ========================================================================
//  COSC363: Computer Graphics (2023);  University of Canterbury.
//
//  FILE NAME: Teapot.cpp
//  See Lab01.pdf for details
//  ========================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <climits>
#include <vector>
#include <math.h> 
#include <GL/freeglut.h>
#include "objects.h"
#include "physics.h"
#include "loadBMP.h"
#include "loadTGA.h"

using namespace std;


// Camera Position data
float cam_hgt = 1.5;
float camX = 0;
float camZ = 10;
// cam look data
float theta = 0;
float lookX = 0;
float lookZ = 0;
float camDistance = 4;

// Scene rendering variables
int view_number = 0;
//Ames window
float AmesRotation = 0;
int animationTime = 5;

// Moire position offset
float mX = 5.0;
float mY = 0.3;
float mZ = 0;
// Moire animation
float moireX = mX;
float moireY = mY - 0.5;
float moireZ = mZ + 0.1;
float moireRotation = 7;

// Newton cradle offset variables
float cradleHeight = 0.7f;
float ball1X = -5.0 + 0.6;
float ball1Y = 0.0 + cradleHeight;
float ball1Z = 0.0;
float ball2X = -5.0 + 0.3;
float ball2Y = 0.0 + cradleHeight;
float ball2Z = 0.0;
float ball3X = -5.0;
float ball3Y = 0.0 + cradleHeight;
float ball3Z = 0.0;
float ball4X = -5.0 - 0.3;
float ball4Y = 0.0 + cradleHeight;
float ball4Z = 0.0;
float ball5X = -5.0 - 0.6;
float ball5Y = 0.0 + cradleHeight;
float ball5Z = 0.0;
float stringLength = 1.7;

float g = -9.81;
float m = 1;			// Ball mass
float a = 0;
bool dropped = false;
int currentDrop = 0;

// Cradle stand
float cradleStandX = ball3X;
float cradleStandY = ball3Y + stringLength;
float cradleStandZ = ball3Z;
// Cradle variables
float acuMomentum = 0;
float averageMomentum = 0;
float ballReleaseHeight = 45;	// Initial release height (degrees)


float light[4] = {-5, 2, 1, 1 };
float shadowMat[16] = { light[1],	0,		0,		0, 
						-light[0],	0, -light[2],	-1, 
							0,		0,	light[1],	0, 
							0,		0,		0,		light[1] };


ball ball1(ball1X, cradleHeight, 0.0, cradleStandX + 0.6, cradleStandY, light, shadowMat);
ball ball2(ball2X, cradleHeight, 0.0, cradleStandX + 0.3, cradleStandY, light, shadowMat);
ball ball3(ball3X, cradleHeight, 0.0, cradleStandX		, cradleStandY, light, shadowMat);
ball ball4(ball4X, cradleHeight, 0.0, cradleStandX - 0.3, cradleStandY, light, shadowMat);
ball ball5(ball5X, cradleHeight, 0.0, cradleStandX - 0.6, cradleStandY, light, shadowMat);
cradleBalls newtonsCradle(ball3X, ball3Y + stringLength, ball3Z, cradleHeight);

//Spotlights rotation
float spotlightTheta = 0;
float spotlightHeight = 3;

// Texture stuff
GLuint txId[3];
// Wall stuff
const int wallPoints = 10;
float wallRadius = 9.0f;
float wallX[wallPoints + 1];
float wallZ[wallPoints + 1];
float wallHeight = 1.0f;
float wallTexH = 4;
float wallR = 70;
// Swept surface stuff
const int sweptPoint = 50;
float frequency = 1;
float sweptVX[sweptPoint];
float sweptVY[sweptPoint];

float sweptNX[sweptPoint];
float sweptNY[sweptPoint];


// TEMP VARIABLES
const int NUM_FRAMES = 60; // Number of frames to average over
float fps_accumulator = 0.0f;
float fps_average = 0.0f;
int frame_count = 0;
float current_time = 0.0f;
float previous_time = 0.0f;
float MAX = 100000;

// Moves the cam up and down using keyboard input
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		// move camera forward
		if (view_number == 0) {
			camX += sin(theta) * 0.1;
			camZ -= cos(theta) * 0.1;
		} else {
			camDistance -= 0.1;
		}
		break;
	case GLUT_KEY_DOWN:
		// move camera back
		if (view_number == 0) {
			camX -= sin(theta) * 0.1;
			camZ += cos(theta) * 0.1;
		} else {
			camDistance += 0.1;
		}
		break;
	case GLUT_KEY_LEFT:
		//cam left
		if (view_number == 0) {
			theta -= 0.075;
		} else {
			theta -= 0.075;
		}
		break;
	case GLUT_KEY_RIGHT:
		// cam right
		if (view_number == 0) {
			theta += 0.075;
		} else {
			theta += 0.075;
		}
		break;
	case GLUT_KEY_PAGE_UP:
		cam_hgt += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam_hgt -= 0.1;
		break;
	}

	if (view_number == 0) {
		lookX = camX + 10 * sin(theta);
		lookZ = camZ - 10 * cos(theta);
	}

	glutPostRedisplay();
}


// Moves to the art places
void keyboard_input(unsigned char key, int x, int y)
{
	switch (key) {
		case '0':
			// Gallery view
			view_number = 0;
			cam_hgt = 1;
			camX = 0;
			camZ = 10;
			theta = 0;
			lookX = camX + 10 * sin(theta);
			lookZ = camZ - 10 * cos(theta);
			glutPostRedisplay();
			break;
		case '1':
			// AAO-1 view

			theta = 0;
			lookX = 0;
			lookZ = 0;

			view_number = 1;
			cam_hgt = 1;
			theta = 0;
			glutPostRedisplay();
			break;
		case '2':
			// AAO-2 view

			lookX = mX + 1;
			cam_hgt = 1;
			lookZ = mZ;

			camX = mX;
			camZ = mZ + 4;

			cam_hgt = 1;
			theta = 0;
			view_number = 2;
			glutPostRedisplay();
			break;
		case '3':
			// AAO-3 view
			lookX = ball3X;
			lookZ = ball3Z;
			camX = ball3X;
			camZ = ball3Z + 2;

			view_number = 3;
			cam_hgt = 1;
			theta = 0;
			glutPostRedisplay();
			break;
		case '4':
			// Reloads ameswindow file for editing it on the fly
			loadAmesFile("AmesWindow.off");
			break;
		case '5':
			wallR -= 1;
			break;
		case '8':
			wallR += 1;
			break;
		case '6':
			wallTexH -= 1;
			break;
		case '9':
			wallTexH += 1;
			break;
	}
}


// Draws the checkered floor for the gallery
void drawFloor()
{
	bool flag = false;
	float floor_height = -0.01;
	float tile_width = 0.2;
	float floor_width = 9;


	float light4_pos[4] = { 0, 6, 0, 1 };
	GLfloat light4ambient[] = { 0.01f, 0.01f, 0.01f, 1.0f };
	GLfloat light4diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light4specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT4, GL_AMBIENT, light4ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light4specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_pos);
	
	glEnable(GL_LIGHT4);

	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	for (float x = -floor_width; x <= floor_width; x += tile_width) {
		for (float z = -floor_width; z <= floor_width; z += tile_width) {
			if (flag) {
				GLfloat color[] = {1.0f, 1.0f, 0.5f, 1.0f};
				glColor3f(color[0], color[1], color[2]);
			}
			else {
				GLfloat color[] = { 1.0f, 0.5f, 1.0f, 1.0f };
				glColor3f(color[0], color[1], color[2]);
			}

			glVertex3f(x, floor_height, z);
			glVertex3f(x, floor_height, z + tile_width);
			glVertex3f(x + tile_width, floor_height, z + tile_width);
			glVertex3f(x + tile_width, floor_height, z);
			flag = !flag;
		}
	}

	glDisable(GL_LIGHT4);
	glEnd();
}


void renderSwept()
{
	float vx[sweptPoint], vy[sweptPoint], vz[sweptPoint];   //vertex positions
	float wx[sweptPoint], wy[sweptPoint], wz[sweptPoint];
	float nx[sweptPoint], ny[sweptPoint], nz[sweptPoint];   //normal vectors
	float mx[sweptPoint], my[sweptPoint], mz[sweptPoint];
	float normalx, normaly;

	// Calculate normals
	for (int i = 0; i < sweptPoint; i++) {
		if (i == 0) //End point of the curve
		{
			normalx = sweptVY[1] - sweptVY[0];
			normaly = -sweptVX[1] + sweptVX[0];
		} else if (i == sweptPoint - 1) //End point of the curve
		{
			normalx = sweptVY[i] - sweptVY[i - 1];
			normaly = -sweptVX[i] + sweptVX[i - 1];
		} else //All interior points
		{
			normalx = sweptVY[i + 1] - sweptVY[i - 1]; //x-component of n1+n2
			normaly = -sweptVX[i + 1] + sweptVX[i - 1]; //y-component of n1+n2
		}
		float dist = sqrt(normalx * normalx + normaly * normaly); //normalization
		normalx /= dist;
		normaly /= dist;
		sweptNX[i] = normalx; //Store values in an array
		sweptNX[i] = normaly;
	}



	for (int i = 0; i < sweptPoint; i++)		//Initialize data everytime the frame is refreshed
	{
		vx[i] = sweptVX[i];
		vy[i] = sweptVY[i];
		vz[i] = 0;
		nx[i] = sweptNX[i];
		ny[i] = sweptNY[i];
		nz[i] = 0;
	}

	glColor3f(1, 0.75, 0.5);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	int nSlices = 36;
	float angStep = 10 * 3.1415 / 180;

	for (int j = 0; j < nSlices; j++) {
		for (int i = 0; i < sweptPoint; i++) {
			wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
			wy[i] = vy[i];
			wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];
		}

		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < sweptPoint; i++) {
			glVertex3f(vx[i], vy[i], vz[i]);
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();

		for (int i = 0; i < sweptPoint; i++)    //Update vertices
		{
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}

	glFlush();
}


void normal(int i)
{
	float xdiff1, zdiff1, xdiff2, zdiff2;
	if (i == 0 || i == 12) glNormal3f(-1, 0, 0);   //default normal along -x direction
	else {
		xdiff1 = wallX[i] - wallX[i - 1];
		zdiff1 = wallZ[i] - wallZ[i - 1];
		xdiff2 = wallX[i + 1] - wallX[i];
		zdiff2 = wallZ[i + 1] - wallZ[i];
		glNormal3f(-(zdiff1 + zdiff2), 0, (xdiff1 + xdiff2));
	}
}


void display(void)
{
	// FPS
	current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float delta_time = current_time - previous_time;
	float fps = 1.0f / delta_time;
	previous_time = current_time;

	// Accumulate the FPS value
	fps_accumulator += fps;
	frame_count++;
	if (frame_count >= NUM_FRAMES) {
		// Calculate the average FPS value
		fps_average = fps_accumulator / NUM_FRAMES;

		// Reset the accumulator and frame count
		fps_accumulator = 0.0f;
		frame_count = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	switch (view_number) {
		case 0:
			gluLookAt(camX, cam_hgt, camZ, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
			break;
		case 1:
			gluLookAt(camDistance * sin(theta), cam_hgt, camDistance * cos(theta), 0, cam_hgt, 0, 0, 1, 0);		//Camera position and orientation
			break;
		case 2:
			gluLookAt(camDistance * sin(theta) + mX + 0.5, cam_hgt, camDistance * cos(theta) + mZ , mX +0.5, cam_hgt, mZ, 0, 1, 0);	//Camera position and orientation
			break;
		case 3:
			gluLookAt(camDistance * sin(theta) + ball3X, cam_hgt, camDistance * cos(theta) + ball3Z, ball3X, cam_hgt, ball3Z, 0, 1, 0);  //Camera position and orientation
			break;
	}
	
	glEnable(GL_LIGHTING);			//Enable lighting

	// Lighting	
	float light0_pos[4] = { 0, 6, 0, 1 };

	// Spotlights
	
	GLfloat light1_pos[4] = { 2 * sin(-spotlightTheta * 0.1), spotlightHeight, 2 * cos(-spotlightTheta * 0.1), 1 };
	float light1_dir[3] = { 0, -1, 0 };
	GLfloat light2_pos[4] = { 2 * sin(spotlightTheta * 0.1), spotlightHeight, 2 * cos(spotlightTheta * 0.1), 1 };
	float light2_dir[3] = { 0, -1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
	

	GLfloat light0ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat light0diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light0specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	
	glDisable(GL_LIGHT0);
	drawFloor();
	glEnable(GL_LIGHT0);

	#pragma region Moire_Illusion
	glPushMatrix();
	glTranslatef(-0.5 + mX, 0 + mY, 0 + mZ);
	glColor3f(0, 0, 0);
	glScalef(0.5, 0.45, 0);
	moire();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0 + mX, 0 + mY, 0 + mZ);
	glColor3f(0, 0, 0);
	glScalef(0.5, 0.45, 0);
	moire();
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.50 + mX, 0 + mY, 0 + mZ);
	glScalef(0.5, 0.45, 0);
	moire();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1 + mX, 0 + mY, 0 + mZ);
	glColor3f(0, 0, 0);
	glScalef(0.5, 0.45, 0);
	moire();
	glPopMatrix();

	// Animated moire part
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(moireX + 0.5, moireY + 1.5, moireZ);
	glRotatef(moireRotation, 0, 0, 1);
	axis();
	glScalef(0.5, 0.55, 1);
	glTranslatef(-0.225, -2.5, 0);
	
	moire();
	
	glPopMatrix();
	#pragma endregion
	

	#pragma region AmesWindow



	//glDisable(GL_LIGHTING);
	// Ames windows

	glPushMatrix();
	glRotatef(AmesRotation, 0, 1, 0);
	glTranslatef(-0.615, 1.5, 0);
	glScalef(0.3,0.5,0.3);
	amesWindow();
	glPopMatrix();

	// Top half
	glPushMatrix();
	glScalef(-1, 1, 1);
	glRotatef(180, 0, 0, 1);
	glRotatef(AmesRotation, 0, 1, 0);
	glTranslatef(-0.615, -1.5, 0);
	glScalef(0.3, 0.5, 0.3);
	amesWindow();
	glPopMatrix();
	#pragma endregion


	axis(1.0f);


	#pragma region StaticTexture

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glTranslatef(3, 0, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
	glVertex3f(-1.5, 0, 0);
	glTexCoord2f(1., 0.);
	glVertex3f(1.5, 0, 0);
	glTexCoord2f(1., 1.);
	glVertex3f(1.5, 3, 0);
	glTexCoord2f(0., 1.);
	glVertex3f(-1.5, 3, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTranslatef(-3, 0, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
	glVertex3f(-1.5, 0, 0);
	glTexCoord2f(1., 0.);
	glVertex3f(1.5, 0, 0);
	glTexCoord2f(1., 1.);
	glVertex3f(1.5, 3, 0);
	glTexCoord2f(0., 1.);
	glVertex3f(-1.5, 3, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	#pragma endregion

	
	#pragma region quadStrip_(Wall)

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	glColor3f(1, 1, 1);
	glLineWidth(1.0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= wallPoints; i++) {
		normal(i);
		glTexCoord2f(i * wallR / (wallPoints), 0);
		glVertex3f(wallX[i], 0, wallZ[i]);
		glTexCoord2f(i * wallR / (wallPoints), wallTexH);
		glVertex3f(wallX[i], wallHeight, wallZ[i]);

	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	#pragma endregion


	#pragma region newtons_cradle

	// 5 balls/string and stand?
	//newtonsCradle.renderBalls();
	ball1.renderBall();
	ball2.renderBall();
	ball3.renderBall();
	ball4.renderBall();
	ball5.renderBall();

	// Top stand
	glPushMatrix();
	glColor3f(0.1,0.1,0.1);
	glScalef(18, 1, 1);
	glTranslatef(cradleStandX/18, cradleStandY, cradleStandZ);
	glutSolidCube(0.1);
	glPopMatrix();
	
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glLineWidth(3.0f);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	
	glVertex3f(cradleStandX + 0.6, cradleStandY, cradleStandZ);
	glVertex3f(ball1.ballX, ball1.ballY, ball1.ballZ);
	glVertex3f(ball2.ballX, ball2.ballY, ball2.ballZ);
	glVertex3f(cradleStandX + 0.3, cradleStandY, cradleStandZ);
	glVertex3f(ball3.ballX, ball3.ballY, ball3.ballZ);
	glVertex3f(cradleStandX, cradleStandY, cradleStandZ);
	glVertex3f(ball4.ballX, ball4.ballY, ball4.ballZ);
	glVertex3f(cradleStandX - 0.3, cradleStandY, cradleStandZ);
	glVertex3f(ball5.ballX, ball5.ballY, ball5.ballZ);
	glVertex3f(cradleStandX - 0.6, cradleStandY, cradleStandZ);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	#pragma endregion


	#pragma region FPS

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 1.0f);
	glRasterPos2f(10, glutGet(GLUT_WINDOW_HEIGHT) - 20);
    char buffer[32];
	snprintf(buffer, 32, "FPS: %.1f", fps_average); // Format the FPS value as a string
    int len = strlen(buffer);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]); // Render each character of the string
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	#pragma endregion

	renderSwept();

	glFlush();
	//glutSwapBuffers();
}


// Timer function for object animations
void animationTimer(int value)
{
	float dt = 0.02;

	AmesRotation += 0.2;

	if (value % 2 == 0) {
		spotlightTheta += 0.1;
	}
	
	if (value % 4 == 0) {
		moireRotation += 1;
	}

	// Newtons cradle
	acuMomentum += abs(ball1.ballAngle) + abs(ball2.ballAngle) + abs(ball3.ballAngle) + abs(ball4.ballAngle) + abs(ball5.ballAngle);
	averageMomentum = acuMomentum * 100 / value;
	if (value > 100) {
		if (averageMomentum < 500.0 && dropped == true) {
			ball1.resetBall();
			ball2.resetBall();
			ball3.resetBall();
			ball4.resetBall();
			ball5.resetBall();
			dropped = false;
			currentDrop ^= 1;
		}
		acuMomentum = 0;
		value = 0;
	}

	if (!dropped) {
		if (ball1.ballAngle <= 45) {
			if (currentDrop == 1) {
				ball2.ballAngle += 0.1;
			}
			ball1.ballAngle += 0.1;

			ball1.update();
			ball2.update();
		} else {
			dropped = true;
		}
	} else {
		//newtonsCradle.updateBalls(dt);
		ball1.updatePosition(dt);
		ball2.updatePosition(dt);
		ball3.updatePosition(dt);
		ball4.updatePosition(dt);
		ball5.updatePosition(dt);

		checkCollisions(&ball1, &ball2, &ball3, &ball4, &ball5);
	}

	glutPostRedisplay();
	value++;
	glutTimerFunc(animationTime, animationTimer, value);
}


void loadTexture()
{
	glGenTextures(3, txId);   //Get 3 texture IDs 
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	loadBMP("staticIllusion1.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadBMP("staticIllusion2.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("Brick_Texture.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void generateWallCoords()
{
	for (int i = 0; i <= wallPoints; i++) {
		wallX[i] = wallRadius * cos((2 * 3.1415 * i / wallPoints));
		wallZ[i] = wallRadius * sin((2 * 3.1415 * i / wallPoints));
	}
}


void generateSweptCoords(float f = frequency)
{
	for (int i = 0; i < sweptPoint; i++) {
		sweptVX[i] = (i / sweptPoint) * 0.5;
		sweptVY[i] = (f * cos((i / sweptPoint) * 0.5));
	}
}





void initialize(void)
{
	float Spotlight1Color[] = {1, 0, 0, 1};		// Red
	float Spotlight2Color[] = { 0, 1, 0, 1 };	// Green

	glClearColor(0.75f, 0.75f, 0.75f, 0.75f); // Background

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);		// Object ambient light
	glEnable(GL_LIGHT1);		// Spotlight 1
	glEnable(GL_LIGHT2);		// Spotloght 2
	glEnable(GL_LIGHT3);		// Shadow light
	glEnable(GL_LIGHT4);		// Floor ambient light

	// Spotlight 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Spotlight1Color);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 50);
	// Spotlight 2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Spotlight2Color);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 50);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// Loads the objects files for rendering
	loadObjects();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 1.0, 1000.0);   //Camera Frustum


	// Newtons cradle initialize
	newtonsCradle.inset(ball1);
	newtonsCradle.inset(ball2);
	newtonsCradle.inset(ball3);
	newtonsCradle.inset(ball4);
	newtonsCradle.inset(ball5);
	newtonsCradle.print();

	// Texture stuff
	loadTexture();
	generateSweptCoords();
	generateWallCoords();

}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Assignment 1");
	
	// SpecialKeys
	glutSpecialFunc(special);
	// Keyboard input
	glutKeyboardFunc(keyboard_input);
	// Timer for animations
	glutTimerFunc(animationTime, animationTimer, 0);

	initialize();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

