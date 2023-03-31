//  ========================================================================
//  COSC363: Computer Graphics (2023);  University of Canterbury.
//
//  FILE NAME: Teapot.cpp
//  See Lab01.pdf for details
//  ========================================================================

#include <iostream>
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

// Scene rendering variables
int view_number = 0;
//Ames window
float AmesRotation = 0;
int amesTimer = 5;
// Moire animation
float moireX = 0;
float moireRotation = 7;
//Spotlights rotation
float spotlightTheta = 0;
float spotlightHeight = 3;

// Moves the cam up and down using keyboard input
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		// move camera forward
		camX += sin(theta) * 0.1;
		camZ -= cos(theta) * 0.1;
		break;
	case GLUT_KEY_DOWN:
		// move camera back
		camX -= sin(theta) * 0.1;
		camZ += cos(theta) * 0.1;
		break;
	case GLUT_KEY_LEFT:
		//cam left
		theta -= 0.075;
		break;
	case GLUT_KEY_RIGHT:
		// cam right
		theta += 0.075;
		break;
	case GLUT_KEY_PAGE_UP:
		cam_hgt++;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam_hgt--;
		break;
	}

	lookX = camX + 10 * sin(theta);
	lookZ = camZ - 10 * cos(theta);

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

			view_number = 2;
			cam_hgt = 1;
			theta = 0;
			glutPostRedisplay();
			break;
		case '3':
			// AAO-3 view

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
	float floor_width = 10;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (float x = -floor_width; x <= floor_width; x += tile_width) {
		for (float z = -floor_width; z <= floor_width; z += tile_width) {
			if (flag) glColor3f(1.0, 1.0, 0.5);
			else glColor3f(1.0, 0.5, 1.0);

			glVertex3f(x, floor_height, z);
			glVertex3f(x, floor_height, z + tile_width);
			glVertex3f(x + tile_width, floor_height, z + tile_width);
			glVertex3f(x + tile_width, floor_height, z);
			flag = !flag;
		}
	}
	glEnd();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	switch (view_number) {
		case 0:
			gluLookAt(camX, cam_hgt, camZ, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
			break;
		case 1:
			gluLookAt(12*sin(theta), cam_hgt, 12*cos(theta), 0, cam_hgt, 0, 0, 1, 0);  //Camera position and orientation
			break;
		case 2:
			gluLookAt(camX, cam_hgt, camZ, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
			break;
		case 3:
			gluLookAt(camX, cam_hgt, camZ, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
			break;
	}
	


	// Lighting
	float defualtLight[] = { 0, 5, 0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, defualtLight);   //Set light position


	float light1_pos[4] = { 2*sin(-spotlightTheta*0.1), spotlightHeight, 2*cos(-spotlightTheta*0.1), 1 };
	float light1_dir[3] = { 0, -1, 0 };
	float light2_pos[4] = { 2 * sin(spotlightTheta * 0.1), spotlightHeight, 2 * cos(spotlightTheta * 0.1), 1 };
	float light2_dir[3] = { 0, -1, 0 };


	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot

	drawFloor();

	/*
	// Moire illusion
	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(0.5, 0.5, 0);
	moire();
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.5, 0, 0);
	glScalef(0.5, 0.5, 0);
	moire();
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(1, 0, 0);
	glScalef(0.5, 0.5, 0);
	moire();
	glPopMatrix();

	// Animated moire part
	glPushMatrix();
	glColor3f(0, 0, 0);
	glRotatef(moireRotation, 0, 0, 1);
	glTranslatef(sin(moireX * 3.1415 / 180), 0, 0.3);
	glScalef(0.5, 0.5, 0);
	moire();
	glPopMatrix();
	
	*/

	
	glDisable(GL_LIGHTING);
	// Ames windows
	glPushMatrix();
	glTranslatef(4.05, 3, 0);
	glRotatef(AmesRotation, 0, 1, 0);
	glTranslatef(-4.05, 0, 0);
	
	amesWindow();
	glPopMatrix();

	// Top half
	glPushMatrix();
	glTranslatef(4.05, 3, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(-AmesRotation, 0, 1, 0);
	glTranslatef(-4.05, 0, 0);
	
	amesWindow();
	glPopMatrix();

	gluSphere(gluNewQuadric(), 0.1, 20, 5);


	glFlush();
}

// Timer function for object animations
void animationTimer(int value)
{
	AmesRotation += 0.2;
	
	moireX += 0.3;
	glutPostRedisplay();

	if (value % 200 == 0) {
		spotlightTheta += 0.05;
	}
	


	// Will be used later for model construction later
	if (value > 100) {
		value = 0;
	} else {
		value++;
	}
	glutTimerFunc(amesTimer, animationTimer, value);
}





//
//----------------------------------------------------------------------
void initialize(void)
{
	float Spotlight1Color[] = {1, 0, 0, 1};		// Red
	float Spotlight2Color[] = { 0, 1, 0, 1 };	// Green


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);		// Light 1
	glEnable(GL_LIGHT1);		// Spotlight 1
	glEnable(GL_LIGHT2);		// Spotloght 2
	glEnable(GL_LIGHT3);		// Shadow light


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

	loadObjects();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 1.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Assignment 1");
	
	// SpecialKeys
	glutSpecialFunc(special);
	// Keyboard input
	glutKeyboardFunc(keyboard_input);
	// Timer for animations
	glutTimerFunc(amesTimer, animationTimer, 0);

	initialize();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

