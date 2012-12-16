#ifndef _COORDINATES_H
#define _COORDINATES_H

#include <iostream>
#include "Constants.h"

using namespace std;

//This class keeps track of x, y, z coordinates of any object.

class Coordinates
{
private:
	float coords[3];				// (x, y, z)
	void swap(Coordinates&, AxisType);
public:
	Coordinates();
	Coordinates(float x, float y, float z);
	Coordinates(const Coordinates &coord);
	
	float get(AxisType axisType);

	void set(AxisType axisType, float value);
	void arrangeAscending(Coordinates &); // Takes to points and makes p1 < p2
	friend ostream& operator<<(ostream& out, Coordinates &coord);
};

Coordinates::Coordinates()
{
	coords[X_AXIS] = coords[Y_AXIS] = coords[Z_AXIS] = 0;
}

Coordinates::Coordinates(float x, float y, float z)
{
	coords[X_AXIS] = x;
	coords[Y_AXIS] = y;
	coords[Z_AXIS] = z;
}
void Coordinates::swap(Coordinates &c2, AxisType axis)
{
	float temp = coords[axis];
		coords[axis] = c2.get(axis);
		c2.set(axis,temp);

}
void Coordinates::arrangeAscending(Coordinates &c2)
{
	if(coords[0]>c2.coords[0])
	{
		swap(c2, X_AXIS);
		swap(c2, Y_AXIS);
		swap(c2, Z_AXIS);
	}
	else
	{
		if(coords[1]>c2.coords[1])
		{
			swap(c2,X_AXIS);
			swap(c2,Y_AXIS);
			swap(c2,Z_AXIS);
		}
		else
		{
			if(coords[2]>c2.coords[2])
			{
				swap(c2,X_AXIS);
				swap(c2,Y_AXIS);
				swap(c2,Z_AXIS);
			}

		}
	}

}
Coordinates::Coordinates(const Coordinates &coord)
{
	coords[X_AXIS] = coord.coords[X_AXIS];
	coords[Y_AXIS] = coord.coords[Y_AXIS];
	coords[Z_AXIS] = coord.coords[Z_AXIS];
}


float Coordinates::get(AxisType axisType)
{
	return coords[axisType];
}


void Coordinates::set(AxisType axisType, float value)
{
	coords[axisType] = value;
}


ostream & operator<<(ostream& out, Coordinates &coord)
{
    out << "(" << coord.coords[X_AXIS] << ", " << coord.coords[Y_AXIS] << ", " << coord.coords[Z_AXIS] << ")";
    return out;
}

#endif