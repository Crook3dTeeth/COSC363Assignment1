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
using namespace std;


// Camera Position data
int cam_hgt = 10;
float theta = 0;


// Mesh file data
float* x, * y, * z;					//vertex coordinates
int* nv, * t1, * t2, * t3, * t4;		//number of vertices and vertex indices of each face
int nvert, nface;					//total number of vertices and faces

// Scene rendering variables
int view_number = 0;

// Moves the cam up and down using keyboard input
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		// move camera forward

		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		// move camera back


		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		//cam left
		theta -= 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		// cam right
		theta += 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_UP:
		cam_hgt++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam_hgt--;
		glutPostRedisplay();
		break;
	}
}


//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
	ifstream fp_in;
	int ne;

	fp_in.open(fname, ios::in);
	if (!fp_in.is_open()) {
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvert >> nface >> ne;			    // read number of vertices, polygons, edges (not used)

	x = new float[nvert];                        //create arrays
	y = new float[nvert];
	z = new float[nvert];

	nv = new int[nface];
	t1 = new int[nface];
	t2 = new int[nface];
	t3 = new int[nface];
	t4 = new int[nface];

	for (int i = 0; i < nvert; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for (int i = 0; i < nface; i++)                         //read polygon list 
	{
		fp_in >> nv[i] >> t1[i] >> t2[i] >> t3[i];
		if (nv[i] == 4)
			fp_in >> t4[i];
		else
			t4[i] = -1;
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}


// Moves to the art places
void keyboard_input(unsigned char key, int x, int y)
{
	switch (key) {
		case '0':
			// Gallery view
			view_number = 0;
			cam_hgt = 10;
			theta = 0;
			glutPostRedisplay();
			break;
		case '1':
			// AAO-1 view
			loadMeshFile("AmesWindow.off");

			view_number = 1;
			cam_hgt = 10;
			theta = 0;
			glutPostRedisplay();
			break;
		case '2':
			// AAO-2 view
			loadMeshFile("square.off");

			view_number = 2;
			cam_hgt = 10;
			theta = 0;
			glutPostRedisplay();
			break;
		case '3':
			// AAO-3 view
			loadMeshFile("square.off");

			view_number = 3;
			cam_hgt = 10;
			theta = 0;
			glutPostRedisplay();
			break;
	}
}

float radians(float degrees)
{
	return degrees * 3.1415 / 180;
}

// Draws the checkered floor for the gallery
void drawFloor()
{
	bool flag = false;
	int floor_height = -1;
	float tile_width = 2;
	float floor_width = 30;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = -floor_width; x <= floor_width; x += tile_width) {
		for (int z = -floor_width; z <= floor_width; z += tile_width) {
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


//--Function to compute the normal vector of a triangle with index indx ----------
void normal(int indx)
{
	float x1 = x[t1[indx]], x2 = x[t2[indx]], x3 = x[t3[indx]];
	float y1 = y[t1[indx]], y2 = y[t2[indx]], y3 = y[t3[indx]];
	float z1 = z[t1[indx]], z2 = z[t2[indx]], z3 = z[t3[indx]];
	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	glNormal3f(nx, ny, nz);
}



void display(void)
{
	float light_pos[4] = { 0., 10., 10., 1.0 };  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	switch (view_number) {
	case 0:
		gluLookAt(0, cam_hgt, 0, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
		break;
	case 1:
		gluLookAt(0, cam_hgt, 0, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
		break;
	case 2:
		gluLookAt(0, cam_hgt, 0, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation

		break;
	case 3:
		gluLookAt(0, cam_hgt, 0, lookX, cam_hgt, lookZ, 0, 1, 0);  //Camera position and orientation
		break;
	}


	// Galary view draw checkered floor
	drawFloor();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);   //Set light position

	drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
	glColor3f(0.0, 1.0, 1.0);


	// Draws objects
	glTranslatef(0, 5, 0);
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

	initialize();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

