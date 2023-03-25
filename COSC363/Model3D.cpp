//  ========================================================================
//  COSC363: Computer Graphics (2023);  University of Canterbury.
//
//  FILE NAME: Teapot.cpp
//  See Lab01.pdf for details
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>

int cam_hgt = 10;
int cam_x = 0;
int cam_z = 0;
int cam_rotation = 0;
float theta = 0;

// Moves the cam up and down using keyboard input
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		// move camera forward
		cam_x++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		// move camera back
		cam_x--;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		//cam left
		cam_rotation++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		// cam right
		cam_rotation--;
		glutPostRedisplay();
		break;
	}
}

// Moves to the art places
void keyboard_input(unsigned char key, int x, int y)
{
	switch (key) {
		case '0':
			// Gallery view
			glutPostRedisplay();
			break;
		case '1':
			// AAO-1 view
			glutPostRedisplay();
			break;
		case '2':
			// AAO-2 view
			glutPostRedisplay();
			break;
		case '3':
			// AAO-3 view
			glutPostRedisplay();
			break;
	}
}



//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5, 0.);			//Floor colour

	for (int i = -50; i <= 50; i++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
		glVertex3f(-50, -0.75, i);
		glVertex3f(50, -0.75, i);
		glVertex3f(i, -0.75, -50);
		glVertex3f(i, -0.75, 50);
		glEnd();
	}
}


//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void)
{
	float lpos[4] = { 0., 10., 10., 1.0 };  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cam_x, cam_hgt, 2, 0, 0, 0, 0, 1, 0);  //Camera position and orientation

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //Set light position

	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
	drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
	glColor3f(0.0, 1.0, 1.0);

	glTranslatef(2, 1, -3);
	glutSolidCube(1);
	glRotatef(30, 0, 1, 0);
	glutSolidTeapot(1);

	glFlush();
}

//----------------------------------------------------------------------
void initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 10.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Teapot");
	// SpecialKeys
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard_input);

	initialize();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

