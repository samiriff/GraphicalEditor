#ifndef _COLOR_H
#define _COLOR_H

#include <GL/glut.h>

class Color
{
private:
	float red;
	float green;
	float blue;
		
	float transparencyValue;

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

	void setTransparencyValue(float value);
};

Color::Color()
{	
	red = green = blue = 0;
	transparencyValue = 1;
}
Color::Color(const Color &c)
{
	red = c.red;
	green = c.green;
	blue = c.blue;
	transparencyValue = c.transparencyValue;
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

	transparencyValue = 1;
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
	glColor4f(red, green, blue, transparencyValue);
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

void Color::setTransparencyValue(float value)
{
	transparencyValue = value;
}


#endif