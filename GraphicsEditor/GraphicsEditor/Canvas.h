#ifndef _CANVAS_H
#define _CANVAS_H
#include "Coordinates.h"
#include "Constants.h"
#include "Tool.h"

#include<string.h>
#include "ColorPanel.h"
#include "UndoRedo.h"
#include<gl/glut.h>

//class Tool;
class Canvas
{
private:
	Coordinates bottom_left;
	Coordinates top_right;	
	
	Coordinates firstPoint;			//The first point clicked by the user
	bool isFirstEntry;
	inline void readFileName();
	GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]; //An array to hold the drawn data same as our my_package.cpp

	bool isCanvasModified;

	History *recentHistory;
public:
	

	Canvas(float, float , float , float);
	void drawBoard();
	bool isClickInside(int , int);

	bool saveToFile(const char *fileName);
	bool LoadFromFile(const char *fileName);

	void drawWithTool(Tool *tool, Color color, int x, int y);
	Coordinates getFirstPoint();	

	bool hasCanvasChanged();			//This is a test-and-set method
	void recordInHistory();
	void restoreOld();
	void restoreNew();
	
	~Canvas();
	
};
Canvas::~Canvas()
{
	//delete fileName;
}

Canvas::Canvas(float x1, float y1, float x2, float y2)
{
	bottom_left.set(X_AXIS, x1);
	bottom_left.set(Y_AXIS, y1);

	top_right.set(X_AXIS, x2);
	top_right.set(Y_AXIS, y2);

	for(int i=0;i<APPLICATION_WINDOW_HEIGHT;i++)
		for(int j=0;j<(APPLICATION_WINDOW_WIDTH*MULT_FACTOR);j++)
			imageData[i][j] = 1;

	isFirstEntry = false;

	recentHistory = new History(NUM_PAST_ENTRIES);
	recentHistory->record(imageData);
	LOG("Recorded BLANK");
	isCanvasModified = false;
}

bool Canvas::isClickInside(int x, int y)
{
	if((x>=bottom_left.get(X_AXIS))&&(x<=top_right.get(X_AXIS)))
		return (y>=bottom_left.get(Y_AXIS))&&(y<=top_right.get(Y_AXIS));
	else
		return false;
}
void Canvas::drawBoard()
{
	glColor3f(1,1,1);
	glRectf(this->bottom_left.get(X_AXIS), this->bottom_left.get(Y_AXIS), top_right.get(X_AXIS), top_right.get(Y_AXIS));

	glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
	glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageData);
}

void Canvas::drawWithTool(Tool *tool, Color color, int x, int y)
{
	firstPoint.set(X_AXIS, x);
	firstPoint.set(Y_AXIS, y);

	color.setGLColor();	
	tool->drawOnCanvas(this, imageData, x, y);			

	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, imageData);	
	isCanvasModified = true;
}

Coordinates Canvas::getFirstPoint()
{
	return firstPoint;
}

void Canvas::readFileName()
{
	cout<<"Enter the file name"<<endl;
}

bool Canvas::LoadFromFile(const char *fileName)
{
	ifstream ifile;
	ifile.open(fileName, ios::in);
	LOG(fileName);
	if(ifile.is_open())
	{
		ifile.read((char *)imageData, sizeof(imageData));
		drawBoard();
		return true;
	}
	else
	{
		LOG("Unable to read the File");
		return false;
	}	
}
bool Canvas::saveToFile(const char *fileName)
{
	ofstream outFile;
	outFile.open(fileName, ios::out | ios::trunc);
	if(outFile.is_open())
	{
		if(!outFile.is_open())
			outFile.open(fileName, ios::out | ios::trunc);
		outFile.write((char *) imageData, sizeof(imageData));
		drawBoard();
		return true;
	}
	else
	{
		LOG("File Not Found :P ");
		return false;
	}
}

void Canvas::recordInHistory()
{
	recentHistory->record(imageData);
}

void Canvas::restoreOld()
{
	recentHistory->undo(imageData);
}

void Canvas::restoreNew()
{
	recentHistory->redo(imageData);
}

bool Canvas::hasCanvasChanged()
{
	bool flag = isCanvasModified;
	isCanvasModified = false;
	return flag;
}

#endif