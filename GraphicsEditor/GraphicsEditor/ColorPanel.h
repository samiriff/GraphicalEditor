#ifndef _COLOR_PANEL_H
#define _COLOR_PANEL_H
#include "Coordinates.h"
#include "Canvas.h"
#include "Constants.h"
#include "Color.h"
#include<gl/glut.h>
#include<iostream>
using namespace std;


class ColorPanel
{
private:
	Coordinates *bottom_left, *top_right;
	Color *gridColors[COLOR_PANEL_NUM_ROWS][COLOR_PANEL_NUM_COLS];	
	Color *selectedColor;
	
	void initGridColors();				//Initializes a 2-D matrix of possible colors that can be selected from the grid, using objects of the Color class
	void addColorSquares();				//Draws color squares using values from the grid of colors	

	bool isClickInsideGrid(int x, int y);
	bool isClickInsideTriangle(int x, int y);

	void setColor(ColorType color);

public:
	ColorPanel(float, float , float , float);
	void drawPanel();

	bool isClickInside(int, int);
	void selectClickedColorFromGrid(int x, int y);
	void selectClickedColorFromTriangle(int x, int y);
		
	Color getSelectedColor();
};

ColorPanel::ColorPanel(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);

	selectedColor = new Color();

	initGridColors();
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

void ColorPanel::initGridColors()
{
	gridColors[0][0] = new Color(1, 0, 0);	
	gridColors[0][1] = new Color(0, 1, 0);	
	gridColors[0][2] = new Color(0, 0, 1);	

	gridColors[1][0] = new Color(0, 1, 1);	
	gridColors[1][1] = new Color(1, 1, 0);	
	gridColors[1][2] = new Color(1, 1, 1);	

	gridColors[2][0] = new Color(0, 0.4, 1);	
	gridColors[2][1] = new Color(0.5,1,0.2);	
	gridColors[2][2] = new Color(1,0.3,1);	

	gridColors[3][0] = new Color(0.932,0.4,1);
	gridColors[3][1] = new Color(0.32,0.378,0.2);
	gridColors[3][2] = new Color(0.654,0.3,0.123);

	gridColors[4][0] = new Color(0,0.4,0.234);
	gridColors[4][1] = new Color(0.5,0.77,0.2);
	gridColors[4][2] = new Color(0.9,0.3,0.2);
}

void ColorPanel::addColorSquares()
{
	float begin_x = bottom_left->get(X_AXIS), begin_y = top_right->get(Y_AXIS);
	
	//Draws a 5x3 grid of colors, that can be selected
	for(int i=0; i<COLOR_PANEL_NUM_ROWS; i++)
	{
		for(int j=0; j<COLOR_PANEL_NUM_COLS; j++)
		{
			gridColors[i][j]->setGLColor();
			glRectf(begin_x,begin_y-COLOR_SQUARE_SIDE, begin_x+COLOR_SQUARE_SIDE, begin_y);
			begin_x += COLOR_SQUARE_SIDE;
		}
		begin_x = bottom_left->get(X_AXIS);
		begin_y -= COLOR_SQUARE_SIDE;
	}
	
	//For the big sqaure
	glColor3f(0,0,0);
	begin_x = bottom_left->get(X_AXIS) + 15;	
	begin_y -= (COLOR_SQUARE_SIDE+GAP);  	

	selectedColor->setGLColor();	
	glRectf(begin_x, begin_y-50, begin_x+50, begin_y);

	//For the triangle you told about
	glColor3f(0.23,0.432,0.567);  

	glPushMatrix();
	
	//glScalef(1, 4.5, 1);	
	//RGB Square
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) + 0);
		glColor3f(0, 0, 1);
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS) + 0);
		glColor3f(0, 1, 0);
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS) + 180);
		glColor3f(0, 0, 0);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) + 180);
	glEnd();
	//CMY Square
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) + 180);
		glColor3f(0, 1, 1);
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS) + 180);
		glColor3f(1, 0, 1);
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS) + 360);
		glColor3f(1, 1, 1);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) + 360);
	glEnd();

	glPopMatrix();
	
}


bool ColorPanel::isClickInsideGrid(int x, int y)
{
	if((x >= bottom_left->get(X_AXIS))&&(x <= bottom_left->get(X_AXIS) + COLOR_SQUARE_SIDE * COLOR_PANEL_NUM_COLS))
		return (y < top_right->get(Y_AXIS))&&(y > top_right->get(Y_AXIS) - COLOR_SQUARE_SIDE * COLOR_PANEL_NUM_ROWS);
	else
		return false;	
}

bool ColorPanel::isClickInsideTriangle(int x, int y)
{
	if((x >= bottom_left->get(X_AXIS))&&(x <= top_right->get(X_AXIS)))
		return (y < bottom_left->get(Y_AXIS) + 360) && (y > bottom_left->get(Y_AXIS) + 30);
	else
		return false;	
}

void ColorPanel::selectClickedColorFromGrid(int x, int y)
{	
	if(isClickInsideGrid(x, y))
	{
		int row = (top_right->get(Y_AXIS) - y) / COLOR_SQUARE_SIDE;
		int col = (x - bottom_left->get(X_AXIS)) / COLOR_SQUARE_SIDE;
		cout << "Row = " << row << "\tCol = " << col << endl;		
		selectedColor = gridColors[row][col];		
	}	
}

void ColorPanel::selectClickedColorFromTriangle(int x, int y)
{	
	if(isClickInsideTriangle(x, y))
	{		
		GLfloat rgbValues[3];
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, rgbValues);
		cout << rgbValues[0];

		delete selectedColor;
		selectedColor = new Color(rgbValues[0], rgbValues[1], rgbValues[2]);
	}
}

Color ColorPanel::getSelectedColor()
{
	return *selectedColor;
}

//For easy management of glColor3f
void ColorPanel::setColor(ColorType color)
{
	switch(color)
	{
	case BLACK:
		glColor3f(0, 0, 0);
		break;
	case RED:
		glColor3f(1, 0, 0);
		break;
	case GREEN:
		glColor3f(0, 1, 0);
		break;
	case BLUE:
		glColor3f(0, 0, 1);
		break;
	case CYAN:
		glColor3f(0, 1, 1);
		break;
	case MAGENTA:
		glColor3f(1, 0, 1);
		break;
	case YELLOW:
		glColor3f(1, 1, 0);
		break;
	case WHITE:
		glColor3f(1, 1, 1);
		break;
	}
}

#endif