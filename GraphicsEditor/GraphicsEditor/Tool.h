#ifndef _TOOL_H
#define _TOOL_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Coordinates.h"
#include "Canvas.h"
#include "Color.h"


#include <GL/glut.h>
using namespace std;


class Canvas;


class Tool
{
private:

protected:
	Canvas *canvas;
	Coordinates *bottom_left, *top_right;
	GLfloat pointSize;
	Size selectedSize;
	//These variables are required for tools that render items as the user moves the mouse
	Coordinates firstPoint;
	bool isFirstPointSelected;
	
	GLfloat imageDataBefore[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR];

	void drawText(const char *info, float x, float y);
	void copyFromTo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], GLfloat buffer[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]);
	
	void getCurrentColor(Color &selectedColor);


public:	
	Tool(float x1, float y1, float x2, float y2);
	void IncreasePointSize();
	void DecreasePointSize();
	void drawPointSizeInfo();
	void setDrawingSize(Size );
	virtual void render() = 0;
	virtual void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY) = 0;	
	virtual void start();
	virtual void stop();
	virtual void select();
	virtual void reset(){}
};

//**********************************************************************************************************************//

class Pencil : public Tool
{
public:
	Pencil(float x1, float y1, float x2, float y2);

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

//**********************************************************************************************************************//

class Spray : public Tool
{
public:
	Spray(float x1, float y1, float x2, float y2);

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

//**********************************************************************************************************************//

class Eraser : public Tool
{
public:
	Eraser(float x1, float y1, float x2, float y2);

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};


//**********************************************************************************************************************//
Tool::Tool(float x1, float y1, float x2, float y2)
{
	LOG("Tool Class");
	bottom_left = new Coordinates(x1, y1, 0);
	top_right = new Coordinates(x2, y2, 0);
	isFirstPointSelected = false;
	pointSize = 0.95;
	IncreasePointSize();
	canvas = NULL;
}
void Tool::setDrawingSize(Size size)
{
	selectedSize = size;
}

void Tool::IncreasePointSize()
{

	pointSize += 0.05;
	 drawPointSizeInfo();
}
void Tool::DecreasePointSize()
{
	pointSize -= 0.05;
    drawPointSizeInfo();
	
}
void Tool::drawPointSizeInfo()
{
	glColor4f(1,1,1, 0.1);
	glRectf(POINT_INFO_LEFT,POINT_INFO_BOTTOM,POINT_INFO_RIGHT,POINT_INFO_TOP);
	stringstream ss;
    glPointSize(pointSize);
	ss << setprecision(2) << pointSize;
    string valueString = "" + ss.str();
	const char *info = valueString.c_str();
	glColor4f(0,0,0, 0.5);	
	//drawText(valueString.c_str(),POINT_INFO_X,POINT_INFO_Y);	
	glRasterPos2f(POINT_INFO_X, POINT_INFO_Y);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*info++);
	}
}
void Tool::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
}

void Tool::drawText(const char *info, float x, float y)
{
	glRasterPos2f(x, y);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*info++);
	}
}

void Tool::copyFromTo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], GLfloat buffer[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR])
{	
	int h, w;
	for (h = 0; h < APPLICATION_WINDOW_HEIGHT; h++)
	{
		for (w = 0; w < APPLICATION_WINDOW_WIDTH * 3; w++)
		{					  
			buffer[h][w] = imageData[h][w];				
		}		
	}
	//cout << "h = " << h << "\tw = " << w << endl;
}


void Tool::start()
{
	isFirstPointSelected = true;	
}

void Tool::stop()
{
	isFirstPointSelected = false;	
}

void Tool::getCurrentColor(Color &selectedColor)
{
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
	Color temp(currentColor[0], currentColor[1], currentColor[2]);
	selectedColor = temp;
}

//**********************************************************************************************************************//

Pencil::Pencil(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Pencil::render()
{
	glColor3f(0, 0.5, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Pencil", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Pencil::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Pencil Drawing");

	glBegin(GL_POLYGON);
		glVertex2f(mouseX, mouseY);
		glVertex2f(mouseX, mouseY + 1);
		glVertex2f(mouseX + 5, mouseY + 1);
		glVertex2f(mouseX + 5, mouseY);
	glEnd();		
}

//**********************************************************************************************************************//

Spray::Spray(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{
	selectedSize = SMALL;
}

void Spray::render()
{
	glColor3f(0, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Spray", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 5, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Spray::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	glPointSize(pointSize);
	glLineWidth(pointSize);
	float theta = 0.0;
	float x = 0, y=0;
	int rmax = 5 + selectedSize*10;
	glBegin(GL_POINTS);
	for(int i=0;i<100;i++)
	{
		float r = rand()%rmax;
		theta = rand()%360;
		glVertex2i(mouseX+r*cos(theta), mouseY+r*sin(theta));
	}
	glEnd();

}

//**********************************************************************************************************************//

Eraser::Eraser(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{
	selectedSize = SMALL;
}

void Eraser::render()
{		
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Eraser", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Eraser::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	float width = 25+selectedSize*10;
	float height = 25+selectedSize*10;	
	LOG("Selected Size = " << selectedSize);
	glColor3f(1,1,1);
	glRectf(mouseX-width,mouseY-height,mouseX+width, mouseY+height);
}


//**********************************************************************************************************************//

class PaintBrush :public Tool
{
public:
	PaintBrush(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

PaintBrush::PaintBrush(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void PaintBrush::render()
{
	glColor3f(0.5, 0.2, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	/*glRasterPos2f((top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0);
	glutSolidCube(50);*/
	drawText("Brush", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 30, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void PaintBrush::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	float dist = 5 + selectedSize*10;

	glLineWidth(1+selectedSize);
	glBegin(GL_POLYGON);
	glVertex2i(mouseX-dist, mouseY+dist);
	glVertex2i(mouseX+dist, mouseY-dist);
	glVertex2i(mouseX+dist-(1+selectedSize)*2, mouseY-dist);
	glVertex2i(mouseX-dist-(1+selectedSize)*2, mouseY+dist);
	glEnd();
}

#endif