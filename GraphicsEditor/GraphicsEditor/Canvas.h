#ifndef _CANVAS_H
#define _CANVAS_H
#include "Coordinates.h"
#include "Constants.h"
#include<gl/glut.h>
class Canvas
{
private:
	Coordinates bottom_left;
	Coordinates top_right;
	//float ImageData[][]; An array to hold the drawn data same as our my_package.cpp

public:
	Canvas(float, float , float , float);
	void drawBoard();
	bool isClickInside(int , int);
};
Canvas::Canvas(float x1, float y1, float x2, float y2)
{
	bottom_left.set(X_AXIS, x1);
	bottom_left.set(Y_AXIS, y1);

	top_right.set(X_AXIS, x2);
	top_right.set(Y_AXIS, y2);
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
}

#endif