#ifndef _TOOL_H
#define _TOOL_H

#include "Coordinates.h"
#include "Canvas.h"

#include <iostream>

using namespace std;

class Tool
{
private:

protected:
	Canvas *canvas;
	Coordinates *bottom_left, *top_right;

	void drawText(char *info, float x, float y);

public:	
	Tool(float x1, float y1, float x2, float y2);

	virtual void render() = 0;
	virtual void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY) = 0;
	virtual void select();
};

Tool::Tool(float x1, float y1, float x2, float y2)
{
	cout << "Tool Class";
	bottom_left = new Coordinates(x1, y1, 0);
	top_right = new Coordinates(x2, y2, 0);
}

void Tool::select()
{
	glColor4f(0, 0, 0, 0.1);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
}

void Tool::drawText(char *info, float x, float y)
{
	glRasterPos2f(x, y);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*info++);
	}
}













class Pencil : public Tool
{
public:
	Pencil(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

Pencil::Pencil(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Pencil::render()
{
	glColor3f(0, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("P", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Pencil::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}


















class Spray : public Tool
{
public:
	Spray(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

Spray::Spray(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Spray::render()
{
	glColor3f(0, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("S", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Spray::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}


















class Line : public Tool
{
public:
	Line(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

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

void Line::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}













class Rectangle : public Tool
{
public:
	Rectangle(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

Rectangle::Rectangle(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Rectangle::render()
{	
	glColor3f(1, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("R", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Rectangle::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}














class Circle : public Tool
{
public:
	Circle(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

Circle::Circle(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Circle::render()
{	
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("C", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Circle::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}













class Eraser : public Tool
{
public:
	Eraser(float x1, float y1, float x2, float y2);

	void render();

	void drawOnCanvas(Canvas *canvas, int mouseX, int mouseY);	
};

Eraser::Eraser(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Eraser::render()
{	
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("E", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void Eraser::drawOnCanvas(Canvas *canvas, int mouseX, int mouseY)
{}


#endif