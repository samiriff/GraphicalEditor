#ifndef _TOOL_H
#define _TOOL_H

#include <cmath>
#include <iostream>
#include <sstream>

#include "Coordinates.h"
#include "Canvas.h"



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

class Line : public Tool
{
private:
	

public:
	Line(float x1, float y1, float x2, float y2);

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);		
};

//**********************************************************************************************************************//

class Rect : public Tool
{
public:
	Rect(float x1, float y1, float x2, float y2);

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

//**********************************************************************************************************************//

class Circle : public Tool
{
public:
	Circle(float x1, float y1, float x2, float y2);

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
	stringstream ss;
    glPointSize(pointSize);
	ss << pointSize;
    string valueString = "Point Size " + ss.str();
	glColor3f(0,0,0);
	/*for(int i=0; i<valueString.length(); i++)
	   glutStrokeCharacter(GLUT_STROKE_ROMAN, valueString[i]);*/
	drawText(valueString.c_str(),POINT_INFO_X,POINT_INFO_Y);
}
void Tool::select()
{
	glColor4f(0, 0, 0, 0.1);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
}

void Tool::drawText(const char *info, float x, float y)
{
	glRasterPos2f(x, y);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*info++);
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

Pencil::Pencil(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Pencil::render()
{
	glColor3f(0, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("P", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Pencil::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Pencil Drawing");

	glBegin(GL_POLYGON);
		glVertex2f(mouseX, mouseY);
		glVertex2f(mouseX, mouseY + 1.8);
		glVertex2f(mouseX + 1.8, mouseY + 1.8);
		glVertex2f(mouseX + 1.8, mouseY);
	glEnd();		
}

Spray::Spray(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Spray::render()
{
	glColor3f(0, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("S", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
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

Line::Line(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Line::render()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), top_right->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS));
	glEnd();

	glColor3f(0, 0, 0);
	drawText("L", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Line::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{	
	glLineWidth(pointSize);
	if(isFirstPointSelected)
	{
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		glPointSize(3);
		glBegin(GL_POINTS);
			glVertex2f(mouseX, mouseY);
		glEnd();

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{		
		//Render previous screen shot
		/*
				There's some bug in this code...Couldn't figure out the problem...Canvas behaves crazily.
				If it works, this code might be more efficient.
		Coordinates lineBoundingBox_bottomLeft(min((int)firstPoint.get(X_AXIS), mouseX), min((int)firstPoint.get(Y_AXIS), mouseY), 0);
		Coordinates lineBoundingBox_topRight(max((int)firstPoint.get(X_AXIS), mouseX), max((int)firstPoint.get(Y_AXIS), mouseY), 0);

		cout << "Bounding box" << "\t\t" << lineBoundingBox_bottomLeft << "\t" << lineBoundingBox_topRight;

		glRasterPos2i(lineBoundingBox_bottomLeft.get(X_AXIS), lineBoundingBox_bottomLeft.get(Y_AXIS));					
		glDrawPixels(lineBoundingBox_topRight.get(X_AXIS) - lineBoundingBox_bottomLeft.get(X_AXIS), lineBoundingBox_topRight.get(Y_AXIS) - lineBoundingBox_bottomLeft.get(Y_AXIS), GL_RGB,GL_FLOAT, imageDataBefore);*/

		
		//This causes some lag....
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);

		glBegin(GL_LINES);
			glVertex2f(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
			glVertex2f(mouseX, mouseY);
		glEnd();	

		//glutPostRedisplay();
	}
}

Rect::Rect(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Rect::render()
{	
	glColor3f(1, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("R", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Rect::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{	glPointSize(pointSize);
	glLineWidth(pointSize);
	if(isFirstPointSelected)
	{		
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{			
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);

		glRectf(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), mouseX, mouseY);		
	}	
}

Circle::Circle(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Circle::render()
{	
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Circle", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 16, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Circle::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	if(isFirstPointSelected)
	{		
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{			
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		float dx = (firstPoint.get(X_AXIS) - mouseX);
		float dy = (firstPoint.get(Y_AXIS) - mouseY);
		float theta = 0;
		float xc = (firstPoint.get(X_AXIS)+mouseX)/2, yc = (firstPoint.get(Y_AXIS) + mouseY)/2; 
		float r = sqrt(dx*dx + dy*dy)/2;
		float x=0,y=0;
		while(theta <= 360)
		{
			x = r*cos(theta);
			y = r*sin(theta);

			glBegin(GL_POINTS);
			glVertex2f(x+xc,y+yc);
			glEnd();
			theta += 0.25;
		}
				
	}	
}

Eraser::Eraser(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Eraser::render()
{	
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Er", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Eraser::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	float width = 25+selectedSize*10;
	float height = 25+selectedSize*10;
	glColor3f(1,1,1);
	glRectf(mouseX-width,mouseY-height,mouseX+width, mouseY+height);
}


class WireCube :public Tool
{
public:
	WireCube(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

WireCube::WireCube(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void WireCube::render()
{
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	/*glRasterPos2f((top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0);
	glutSolidCube(50);*/
	drawText("Cube", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 30, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void WireCube::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	//glRasterPos2i(mouseX,mouseY);
	//glutWireCube(10);
	glBegin(GL_LINES);
	glVertex2i(mouseX, mouseY);
	glVertex2i(mouseY, mouseX);
	glEnd();
}

#endif