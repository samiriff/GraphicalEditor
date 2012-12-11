#ifndef _COLOR_PANEL_H
#define _COLOR_PANEL_H
#include "Coordinates.h"
#include "Canvas.h"
#include<gl/glut.h>
#include<iostream>
using namespace std;


class ColorPanel
{
private:
	Coordinates *bottom_left, *top_right;
	void addColorSquares();
public:
	ColorPanel(float, float , float , float);
	void drawPanel();
	bool isClickInside(int, int);

};
ColorPanel::ColorPanel(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
}
bool ColorPanel::isClickInside(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		return (y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS));
	else
		return false;
}
void ColorPanel::drawPanel()
{
	glColor3f(1,1,1);
	//glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glBegin(GL_LINE_LOOP);
	glVertex2f(bottom_left->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),top_right->get(Y_AXIS));
	glVertex2f(bottom_left->get(X_AXIS),top_right->get(Y_AXIS));
	glEnd();
	addColorSquares();
}
void ColorPanel::addColorSquares()
{
	float begin_x = bottom_left->get(X_AXIS), begin_y = top_right->get(Y_AXIS);
	//Row 1
	glColor3f(1,0,0);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(0,1,0);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(0,0,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	
	
	//Row 2
	begin_x = bottom_left->get(X_AXIS);
	begin_y -= COLOR_SQUARE_SIDE;
	glColor3f(0,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(1,1,0);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);

	//Row 3
	begin_x = bottom_left->get(X_AXIS);
	begin_y -= COLOR_SQUARE_SIDE;
	glColor3f(0,0.4,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(0.5,1,0.2);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(1,0.3,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);

		//Row 4
	begin_x = bottom_left->get(X_AXIS);
	begin_y -= COLOR_SQUARE_SIDE;
	glColor3f(0.932,0.4,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(0.32,0.378,0.2);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(0.654,0.3,0.123);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);

		//Row 5
	begin_x = bottom_left->get(X_AXIS);
	begin_y -= COLOR_SQUARE_SIDE;
	glColor3f(0,0.4,0.234);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;

	glColor3f(0.5,0.77,0.2);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(0.9,0.3,0.2);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);


	glColor3f(0,0,0);
	begin_x = bottom_left->get(X_AXIS) + 15;
	
	begin_y -= (COLOR_SQUARE_SIDE+GAP);  //For the big sqaure
	glRectf(begin_x, begin_y-50, begin_x+50, begin_y);
	
	glColor3f(0.23,0.432,0.567);  // for the triangle you told about
	 begin_x = bottom_left->get(X_AXIS), begin_y = bottom_left->get(Y_AXIS);
	glBegin(GL_TRIANGLES);
	glVertex2f(begin_x+10, begin_y+30);
	glVertex2f(begin_x+60, begin_y+30);
	glVertex2f(begin_x+35, begin_y+60);
	glEnd();

}


#endif