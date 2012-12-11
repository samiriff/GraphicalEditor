#ifndef _COLOR_H
#define _COLOR_H

#include <GL/glut.h>

class Color
{
private:
	float red;
	float green;
	float blue;

public:
	Color();
	Color(float r, float g, float b);
	void setGLColor();	

	friend ostream& operator<<(ostream& out, Color &color);
};

Color::Color()
{
	red = green = blue = 0;
}

Color::Color(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

void Color::setGLColor()
{
	glColor3f(red, green, blue);
}

ostream & operator<<(ostream& out, Color &color)
{
    out << "(" << color.red << ", " << color.green << ", " << color.blue << ")";
    return out;
}

#endif