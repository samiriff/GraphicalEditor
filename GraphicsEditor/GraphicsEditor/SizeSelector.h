#include "Constants.h"
#include <gl/glut.h>
#include "Coordinates.h"

class SizeSelector
{
	Size selectedSize;
	Coordinates *bottom_left, *top_right;
	void updateSelectionBox();
	void drawText(const char *info, float x, float y);
public:
	SizeSelector(float, float, float, float);
	void draw();
	void updateXYInfo(int, int);
	bool isClickInsideSizeSelector(int , int);
	Size selectClickedSize(int , int);
	Size getSelectedSize();
};
SizeSelector::SizeSelector(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
	selectedSize = SMALL;
	updateSelectionBox();
}
bool SizeSelector::isClickInsideSizeSelector(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		if((y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS)))
		{
			selectedSize = selectClickedSize(x,y);
			return true;
		}
	else
		return false;
}
Size SizeSelector::getSelectedSize()
{
	return selectedSize;
}
void SizeSelector::drawText(const char *info, float x, float y)
{
	glRasterPos2f(x, y);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*info++);
	}
}

void SizeSelector::updateXYInfo(int mouseX, int mouseY)
{
	glColor3f(1,1,1);
	glRectf(XY_INFO_LEFT,XY_INFO_BOTTOM,XY_INFO_RIGHT,XY_INFO_TOP);
	stringstream ss;
	ss << mouseX<<","<<mouseY;
    string valueString = "(" + ss.str()+")";
	glColor3f(0,0,0);
	const char *info = valueString.c_str();
	//drawText(valueString.c_str(),XY_INFO_LEFT,XY_INFO_BOTTOM);		
	glRasterPos2f(XY_INFO_LEFT, XY_INFO_BOTTOM);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*info++);
	}	
}
void SizeSelector::updateSelectionBox()
{
	float begin_x = bottom_left->get(X_AXIS)+selectedSize*COLOR_SQUARE_SIDE, begin_y;

	glColor4f(0, 0, 0, 0.1);			//Using a tranlucent polygon
	glRectf(begin_x, bottom_left->get(Y_AXIS), begin_x+COLOR_SQUARE_SIDE, top_right->get(Y_AXIS));
}
Size SizeSelector::selectClickedSize(int x, int y)
{
	int size = (x-bottom_left->get(X_AXIS))/COLOR_SQUARE_SIDE;
	switch(size)
	{
	case SMALL:
		return SMALL;
	case MEDIUM:
		return MEDIUM;
	case LARGE:
		return LARGE;
	}
}
void SizeSelector::draw()
{
	float begin_x, begin_y;
	begin_x = bottom_left->get(X_AXIS);
	begin_y =  bottom_left->get(Y_AXIS);
	glPushMatrix();
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y, begin_x+COLOR_SQUARE_SIDE, begin_y+COLOR_SQUARE_SIDE);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2i(begin_x+10, begin_y+10);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-10, begin_y+10);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-10, begin_y+COLOR_SQUARE_SIDE-10);
	glVertex2i(begin_x+10, begin_y+COLOR_SQUARE_SIDE-10);
	glEnd();
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y, begin_x+COLOR_SQUARE_SIDE, begin_y+COLOR_SQUARE_SIDE);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2i(begin_x+7, begin_y+7);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-7, begin_y+7);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-7, begin_y+COLOR_SQUARE_SIDE-7);
	glVertex2i(begin_x+7, begin_y+COLOR_SQUARE_SIDE-7);
	glEnd();
	
	begin_x += COLOR_SQUARE_SIDE;
	
	glColor3f(1,1,1);
	glRectf(begin_x,begin_y, begin_x+COLOR_SQUARE_SIDE, begin_y+COLOR_SQUARE_SIDE);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2i(begin_x+5, begin_y+5);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-5, begin_y+5);
	glVertex2i(begin_x+COLOR_SQUARE_SIDE-5, begin_y+COLOR_SQUARE_SIDE-5);
	glVertex2i(begin_x+5, begin_y+COLOR_SQUARE_SIDE-5);
	glEnd();
	glPopMatrix();
	updateSelectionBox();
}