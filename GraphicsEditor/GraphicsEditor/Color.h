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
	bool operator == (Color &);
	friend ostream& operator<<(ostream& out, Color &color);
	float getGreen();
	float getRed();
	float getBlue();
};

Color::Color()
{
	red = green = blue = 0;
}
bool Color::operator == (Color &c)
{
	return (red == c.getRed())&&(green==c.getGreen())&&(blue==c.getBlue());
}
Color::Color(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}
float Color::getRed()
{
	return red;
}

float Color::getGreen()
{
	return green;
}

float Color::getBlue()
{
	return blue;
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