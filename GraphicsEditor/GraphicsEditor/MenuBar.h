#include "Coordinates.h"
#include "Constants.h"
#include<gl/glut.h>


class MenuBar
{
private:
		Coordinates *bottom_left, *top_right;
		void initMenuOptions();
public:
	MenuBar( float,float,float,float);
	void addMenuBar();
};
MenuBar::MenuBar(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
}

void MenuBar::addMenuBar()
{
	glColor3f(1,1,1);
	//glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glBegin(GL_LINE_LOOP);
	glVertex2f(bottom_left->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),bottom_left->get(Y_AXIS));
	glVertex2f(top_right->get(X_AXIS),top_right->get(Y_AXIS));
	glVertex2f(bottom_left->get(X_AXIS),top_right->get(Y_AXIS));
	glEnd();
}
