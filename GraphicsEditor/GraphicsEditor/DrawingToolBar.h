#include "Coordinates.h"
#include "Constants.h"
#include<gl/glut.h>


class DrawingToolBar
{
private:
	Coordinates *bottom_left, *top_right;
	void drawTools();
public:
	DrawingToolBar(float, float , float , float);
	void drawToolBar();
	bool isClickInside(int , int);
};
bool DrawingToolBar::isClickInside(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		return (y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS));
	else
		return false;
}
DrawingToolBar::DrawingToolBar(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
}

void DrawingToolBar::drawToolBar()
{
	glColor3f(1,1,1);
	//glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glBegin(GL_LINE_LOOP);
	glVertex2f(bottom_left->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),top_right->get(Y_AXIS));
	glVertex2f(bottom_left->get(X_AXIS),top_right->get(Y_AXIS));
	glEnd();
	drawTools();
}

void DrawingToolBar::drawTools()
{
	float begin_x = bottom_left->get(X_AXIS)+GAP, begin_y = top_right->get(Y_AXIS)-GAP;
	//These are for icons like pencil, spray, etc...
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	//ROW NO 2
	begin_x = bottom_left->get(X_AXIS)+GAP;
	begin_y -= (COLOR_SQUARE_SIDE+GAP);
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;

	glColor3f(1,1,1);
	glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
	begin_x += COLOR_SQUARE_SIDE+GAP;
}