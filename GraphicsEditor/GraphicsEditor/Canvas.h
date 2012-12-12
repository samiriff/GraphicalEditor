#ifndef _CANVAS_H
#define _CANVAS_H
#include "Coordinates.h"
#include "Constants.h"
#include "Tool.h"
#include "ColorPanel.h"
#include<gl/glut.h>

//class Tool;
class Canvas
{
private:
	Coordinates bottom_left;
	Coordinates top_right;	

	Coordinates firstPoint;			//The first point clicked by the user
	bool isFirstEntry;

	GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]; //An array to hold the drawn data same as our my_package.cpp
public:
	

	Canvas(float, float , float , float);
	void drawBoard();
	bool isClickInside(int , int);

	void drawWithTool(Tool *tool, Color color, int x, int y);
	Coordinates getFirstPoint();	
	
};


Canvas::Canvas(float x1, float y1, float x2, float y2)
{
	bottom_left.set(X_AXIS, x1);
	bottom_left.set(Y_AXIS, y1);

	top_right.set(X_AXIS, x2);
	top_right.set(Y_AXIS, y2);

	isFirstEntry = false;

	//glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT, CANVAS_TOP, GL_RGB, GL_FLOAT, imageData);	
}

bool Canvas::isClickInside(int x, int y)
{
	if((x>=bottom_left.get(X_AXIS))&&(x<=top_right.get(X_AXIS)))
		return (y>=bottom_left.get(Y_AXIS))&&(y<=top_right.get(Y_AXIS));
	else
		return false;
}
void Canvas::drawBoard()
{
	glColor3f(1,1,1);
	glRectf(this->bottom_left.get(X_AXIS), this->bottom_left.get(Y_AXIS), top_right.get(X_AXIS), top_right.get(Y_AXIS));

	glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
	glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageData);
}

void Canvas::drawWithTool(Tool *tool, Color color, int x, int y)
{
	firstPoint.set(X_AXIS, x);
	firstPoint.set(Y_AXIS, y);

	color.setGLColor();	
	tool->drawOnCanvas(this, imageData, x, y);		

	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, imageData);	
}

Coordinates Canvas::getFirstPoint()
{
	return firstPoint;
}

/*GLfloat Canvas::(*getImageData())[APPLICATION_WINDOW_WIDTH * 3]
{
	return imageData;
}*/


#endif