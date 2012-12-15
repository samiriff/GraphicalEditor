#include<iostream>
#include<fstream>
#include "Constants.h"
#include "DrawingBoard.h"
//#include "Coordinates.h"

//#include "Canvas.h"
//#include "Tool.h"
#include<gl/glut.h>

using namespace std;

DrawingBoard drawingBoard;
int reshapeWidth = APPLICATION_WINDOW_WIDTH, reshapeHeight = APPLICATION_WINDOW_HEIGHT;


void display(void);
void myinit(void);
void handleMouseEvent(int,int,int,int);
void handleMouseClickMotion(int x, int y);
void reshape(int, int);
void handleKeyPress(unsigned char , int, int);
void handleSpecialKeyPress(int, int, int);
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(APPLICATION_WINDOW_WIDTH, APPLICATION_WINDOW_HEIGHT);
	glutInitWindowPosition(700,100);
	glutCreateWindow("Jinkchak Paint Package");
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(handleKeyPress);
	glutSpecialFunc(handleSpecialKeyPress);
	glutMouseFunc(handleMouseEvent);
	glutMotionFunc(handleMouseClickMotion);
	myinit();
	glutMainLoop();
}

void handleKeyPress(unsigned char key, int x, int y)
{
	drawingBoard.handleKeyPress(key, x, y);
}

void handleSpecialKeyPress(int key, int x, int y)
{
	drawingBoard.handleSpecialKeyPress(key, x, y);
}

void myinit(void)
{
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

	glClearColor(0.8784,0.8784,0.8784,0.8784);
	glColor3f(1,1,1);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,APPLICATION_WINDOW_WIDTH,0,APPLICATION_WINDOW_HEIGHT);

	glMatrixMode(GL_MODELVIEW);
}

void handleMouseEvent(int button, int state, int x, int y)
{
	float scaleFactorWidth = (float)reshapeWidth / APPLICATION_WINDOW_WIDTH;
	float scaleFactorHeight = (float)reshapeHeight / APPLICATION_WINDOW_HEIGHT;

	x *= scaleFactorWidth;
	y = APPLICATION_WINDOW_HEIGHT - y;
	y *= scaleFactorHeight;
	drawingBoard.handleMouseClick(button, state, x, y);

	glutPostRedisplay();
}

void handleMouseClickMotion(int x, int y)
{
	y = APPLICATION_WINDOW_HEIGHT - y;
	
	drawingBoard.handleMouseClickMotion(x, y);

	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawingBoard.initBoard();	
	glFlush();
}

void reshape(int w, int h)
{
	reshapeWidth = w;
	reshapeHeight = h;
}