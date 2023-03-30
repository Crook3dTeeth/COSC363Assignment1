#include "objects.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include <GL/freeglut.h>
using namespace std;


// Mesh file data
// amesWindow data
float* xAmes, * yAmes, * zAmes;					//vertex coordinates
int* nvAmes, * t1Ames, * t2Ames, * t3Ames, * t4Ames;		//number of vertices and vertex indices of each face
int nvertAmes, nfaceAmes;					//total number of vertices and faces
// Moire data
float* xMoire, * yMoire, * zMoire;					//vertex coordinates
int* nvMoire, * t1Moire, * t2Moire, * t3Moire, * t4Moire;		//number of vertices and vertex indices of each face
int nvertMoire, nfaceMoire;					//total number of vertices and faces



void loadObjects()
{
	loadAmesFile("AmesWindow.off");
	loadMoireFile("moire.off");

}

//--Function to compute the normal vector of a triangle with index indx ----------
void normal(int indx, float* x, float* y, float* z, int t1[], int t2[], int t3[])
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


void loadAmesFile(const char* fname)
{
	ifstream fp_in;
	int ne;

	fp_in.open(fname, ios::in);
	if (!fp_in.is_open()) {
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvertAmes >> nfaceAmes >> ne;			    // read number of vertices, polygons, edges (not used)

	xAmes = new float[nvertAmes];                        //create arrays
	yAmes = new float[nvertAmes];
	zAmes = new float[nvertAmes];

	nvAmes = new int[nfaceAmes];
	t1Ames = new int[nfaceAmes];
	t2Ames = new int[nfaceAmes];
	t3Ames = new int[nfaceAmes];
	t4Ames = new int[nfaceAmes];

	for (int i = 0; i < nvertAmes; i++)                         //read vertex list 
		fp_in >> xAmes[i] >> yAmes[i] >> zAmes[i];

	for (int i = 0; i < nfaceAmes; i++)                         //read polygon list 
	{
		fp_in >> nvAmes[i] >> t1Ames[i] >> t2Ames[i] >> t3Ames[i];
		if (nvAmes[i] == 4)
			fp_in >> t4Ames[i];
		else
			t4Ames[i] = -1;
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

void loadMoireFile(const char* fname)
{
	ifstream fp_in;
	int ne;

	fp_in.open(fname, ios::in);
	if (!fp_in.is_open()) {
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvertMoire >> nfaceMoire >> ne;			    // read number of vertices, polygons, edges (not used)

	xMoire = new float[nvertMoire];                        //create arrays
	yMoire = new float[nvertMoire];
	zMoire = new float[nvertMoire];

	nvMoire = new int[nfaceMoire];
	t1Moire = new int[nfaceMoire];
	t2Moire = new int[nfaceMoire];
	t3Moire = new int[nfaceMoire];
	t4Moire = new int[nfaceMoire];

	for (int i = 0; i < nvertMoire; i++)                         //read vertex list 
		fp_in >> xMoire[i] >> yMoire[i] >> zMoire[i];

	for (int i = 0; i < nfaceMoire; i++)                         //read polygon list 
	{
		fp_in >> nvMoire[i] >> t1Moire[i] >> t2Moire[i] >> t3Moire[i];
		if (nvMoire[i] == 4)
			fp_in >> t4Moire[i];
		else
			t4Moire[i] = -1;
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}


void amesWindow()
{
	for (int indx = 0; indx < nfaceAmes; indx++)		//draw each face
	{
		if (indx > 5) {
			glColor3f(1,0,1);
		} else {
			glColor3f(0, 0, 1);
		}
		normal(indx, xAmes, yAmes, zAmes, t1Ames, t2Ames, t3Ames);
		if (nvAmes[indx] == 3)	glBegin(GL_TRIANGLES);
		else				glBegin(GL_QUADS);
		glVertex3d(xAmes[t1Ames[indx]], yAmes[t1Ames[indx]], zAmes[t1Ames[indx]]);
		glVertex3d(xAmes[t2Ames[indx]], yAmes[t2Ames[indx]], zAmes[t2Ames[indx]]);
		glVertex3d(xAmes[t3Ames[indx]], yAmes[t3Ames[indx]], zAmes[t3Ames[indx]]);
		if (nvAmes[indx] == 4)
			glVertex3d(xAmes[t4Ames[indx]], yAmes[t4Ames[indx]], zAmes[t4Ames[indx]]);
		glEnd();
	}
}

void moire()
{
	for (int indx = 0; indx < nfaceMoire; indx++)		//draw each face
	{
		normal(indx, xMoire, yMoire, zMoire, t1Moire, t2Moire, t3Moire);
		if (nvMoire[indx] == 3)	glBegin(GL_TRIANGLES);
		else				glBegin(GL_QUADS);
		glVertex3d(xMoire[t1Moire[indx]], yMoire[t1Moire[indx]], zMoire[t1Moire[indx]]);
		glVertex3d(xMoire[t2Moire[indx]], yMoire[t2Moire[indx]], zMoire[t2Moire[indx]]);
		glVertex3d(xMoire[t3Moire[indx]], yMoire[t3Moire[indx]], zMoire[t3Moire[indx]]);
		if (nvMoire[indx] == 4)
			glVertex3d(xMoire[t4Moire[indx]], yMoire[t4Moire[indx]], zMoire[t4Moire[indx]]);
		glEnd();
	}
}