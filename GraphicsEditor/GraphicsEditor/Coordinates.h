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

public:
	Coordinates();
	Coordinates(float x, float y, float z);
	Coordinates(const Coordinates &coord);
	
	float get(AxisType axisType);

	void set(AxisType axisType, float value);

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