#ifndef _COORDINATES_H
#define _COORDINATES_H

#include <iostream>
#include <cmath>
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

	/* Takes two points (p1 and p2, where p1 is the object on which the method is called, and p2 is the object passed as a parameter to this method
	 *		Sets p1 to the bottom left coordinate of the bounding box of p1 and p2
	 *		Sets p2 to the top right coordinates of the bounding box of p1 and p2
	*/
	void setToBoundingBoxCoordinates(Coordinates &); 
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
void Coordinates::setToBoundingBoxCoordinates(Coordinates &c2)
{
	Coordinates temp(*this);			//Required because this's values are being changed

	this->coords[X_AXIS] = min(coords[X_AXIS], c2.coords[X_AXIS]);
	this->coords[Y_AXIS] = min(coords[Y_AXIS], c2.coords[Y_AXIS]);

	c2.coords[X_AXIS] = max(temp.coords[X_AXIS], c2.coords[X_AXIS]);
	c2.coords[Y_AXIS] = max(temp.coords[Y_AXIS], c2.coords[Y_AXIS]);		
	
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