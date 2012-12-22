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
	Color(const Color &c);
	void setGLColor();	
	void setInvertedGLColor();
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
Color::Color(const Color &c)
{
	red = c.red;
	green = c.green;
	blue = c.blue;
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

void Color::setInvertedGLColor()
{
	glColor3f(1 - red, 1 - green, 1 - blue);
}

ostream & operator<<(ostream& out, Color &color)
{
    out << "(" << color.red << ", " << color.green << ", " << color.blue << ")";
    return out;
}

#endif