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
#include <math.h> 
#include <GL/freeglut.h>
#include "objects.h"
using namespace std;


// Camera Position data
int cam_hgt = 1;
float camX = 0;
float camZ = 5;
// cam look data
float theta = 0;
float lookX = 0;
float lookZ = 0;
float camDistance = 4;

// Scene rendering variables
int view_number = 3;
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
float ball1M = 0;		// Momentum
float ball2M = 0;
float ball3M = 0;
float ball4M = 0;
float ball5M = 0;
float ball1E = 0;
float ball2E = 0;
float ball3E = 0;
float ball4E = 0;
float ball5E = 0;
float v1 = 0;	// Velocity (angular)
float v2 = 0;
float v3 = 0;
float v4 = 0;
float v5 = 0;
float ball1Angle = 0;	// Angle for initial drop
float ball2Angle = 0;
float ball3Angle = 0;
float ball4Angle = 0;
float ball5Angle = 0;
float g = -9.81;
float m = 1;			// Ball mass
float a = 0;
bool dropped = false;




// Cradle stand
float cradleStandX = ball3X;
float cradleStandY = ball3Y + stringLength;
float cradleStandZ = ball3Z;
// Cradle variables
float cradleRadius = 0.15;
float ballReleaseHeight = 45;	// Initial release height (degrees)



ball ball1(ball1X, cradleHeight, 0.0, cradleStandX + 0.6, cradleStandY);

//Spotlights rotation
float spotlightTheta = 0;
float spotlightHeight = 3;

// TEMP VARIABLES
const int NUM_FRAMES = 60; // Number of frames to average over
float fps_accumulator = 0.0f;
float fps_average = 0.0f;
int frame_count = 0;
float current_time = 0.0f;
float previous_time = 0.0f;

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
		cam_hgt++;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam_hgt--;
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
			theta = 0;
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
			loadAmesFile("AmesWindow.off");
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

	GLfloat light1_pos[4] = { 2*sin(-spotlightTheta*0.1), spotlightHeight, 2*cos(-spotlightTheta*0.1), 1 };
	float light1_dir[3] = { 0, -1, 0 };
	GLfloat light2_pos[4] = { 2 * sin(spotlightTheta * 0.1), spotlightHeight, 2 * cos(spotlightTheta * 0.1), 1 };
	float light2_dir[3] = { 0, -1, 0 };
	float light0_pos[4] = { 0, 6, 0, 1 };

	GLfloat light0ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat light0diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light0specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);


	// Spotlights
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);

	
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
	

	
	//glDisable(GL_LIGHTING);
	// Ames windows
	glPushMatrix();
	glRotatef(AmesRotation, 0, 1, 0);
	glTranslatef(-2.025, 1.5, 0);
	glScalef(0.5,0.5,0.5);
	amesWindow();
	glPopMatrix();

	// Top half
	glPushMatrix();

	glScalef(-1, 1, 1);
	glRotatef(180, 0, 0, 1);
	glRotatef(AmesRotation, 0, 1, 0);
	glTranslatef(-2.025, -1.5, 0);
	glScalef(0.5, 0.5, 0.5);
	amesWindow();
	glPopMatrix();

	axis(1.0f);


	#pragma region newtons_cradle

	// 5 balls/string and stand?
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(ball1.ballX, ball1.ballY, ball1.ballZ);
	glutSolidSphere(cradleRadius, 30, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(ball2X, ball2Y, ball2Z);
	glutSolidSphere(cradleRadius, 30, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(ball3X, ball3Y, ball3Z);
	glutSolidSphere(cradleRadius, 30, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(ball4X, ball4Y, ball4Z);
	glutSolidSphere(cradleRadius, 30, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(ball5X, ball5Y, ball5Z);
	glutSolidSphere(cradleRadius, 30, 10);
	glPopMatrix();

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
	glVertex3f(ball2X, ball2Y, ball2Z);
	glVertex3f(cradleStandX + 0.3, cradleStandY, cradleStandZ);
	glVertex3f(ball3X, ball3Y, ball3Z);
	glVertex3f(cradleStandX, cradleStandY, cradleStandZ);
	glVertex3f(ball4X, ball4Y, ball4Z);
	glVertex3f(cradleStandX - 0.3, cradleStandY, cradleStandZ);
	glVertex3f(ball5X, ball5Y, ball5Z);
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

	glFlush();
}




void checkCollisions()
{
	if (abs(ball1M) > 0) {
		float distance = sqrt(((ball2X - ball1X) * (ball2X - ball1X)) + ((ball2Y - ball1Y) * (ball2Y - ball1Y)));
		if (distance <= 0.3) 			{

			// collision
			if (ball2M > 0) {
				float M = ball2M;
				ball1M =M;
			}
		}
	}

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
	if (ball1E + ball2E + ball3E + ball4E + ball5E <= 0) {
		if (dropped) {
			// Reset balls
			ball1E = m * g * (ball1Y - ball3Y);
		}else if (ball1.ballAngle <= 45) {
			ball1.ballAngle += 0.1;
			ball2Angle += 0.1;

			ball1.update();

			ball1Y = (cradleStandY)+stringLength * -cos((ball1Angle * 3.1415 / 180));
		} else {
			dropped = true;
			ball1E = 3;
			cout << ball1Y << endl;
		}
	} else {

		ball1.updatePosition(dt);


		checkCollisions();
	}

	glutPostRedisplay();
	glutTimerFunc(animationTime, animationTimer, value);
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
	gluPerspective(60, 1, 1.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
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

