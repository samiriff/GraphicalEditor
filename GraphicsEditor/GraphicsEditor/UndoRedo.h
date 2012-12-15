#ifndef _UNDO_REDO_H
#define _UNDO_REDO_H

#include <iostream>
#include <vector>
#include <stack>
#include <gl/glut.h>
#include "Constants.h"

using namespace std;

class History
{
private:
	vector<GLfloat **> past;
	int numItems;

	stack<GLfloat **> redoStack;

	void destroy(GLfloat **data);
	void destroyRedoStack();

public:
	History(int n);

	void record(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]);
	void undo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]);
	void redo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR]);
};

History::History(int n)
{
	numItems = n;	
}

void History::destroy(GLfloat **data)
{
	for(int i=0; i<APPLICATION_WINDOW_HEIGHT; i++)
	{
		delete data[i];
	}
	delete data;
}

void History::destroyRedoStack()
{
	while(!redoStack.empty())
	{
		delete redoStack.top();
		redoStack.pop();
	}
}

void History::record(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR])
{
	LOG("Recording");

	destroyRedoStack();
		
	GLfloat **data = new GLfloat*[APPLICATION_WINDOW_HEIGHT];
	for(int i=0; i<APPLICATION_WINDOW_HEIGHT; i++)
	{
		data[i] = new GLfloat[APPLICATION_WINDOW_WIDTH * MULT_FACTOR];
		for(int j=0; j<APPLICATION_WINDOW_WIDTH * MULT_FACTOR; j++)
		{
			data[i][j] = imageData[i][j];
		}
	}	
		
	if(past.size() >= numItems)
	{
		destroy(past.front());
		past.erase(past.begin());
	}

	past.push_back(data);
	LOG("Current Size of buffer = " << past.size());
}

void History::undo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR])
{
	LOG("Undo");

	if(past.empty() || past.size() == 1)
		return;
	
	//Transfer last element to redo-stack, since this element is currently displayed on the screen
	GLfloat **data = past[past.size() - 1];
	redoStack.push(data);
	past.pop_back();


	//Have to perform this copy because the stupid glReadPixels function doesn't accept GLfloat** variables, only GLfloat[][]
	data = past[past.size() - 1];		
	for(int i=0; i<APPLICATION_WINDOW_HEIGHT; i++)
	{		
		for(int j=0; j<APPLICATION_WINDOW_WIDTH * MULT_FACTOR; j++)
		{
			imageData[i][j] = data[i][j];
		}
	}
		
	LOG("Current Size of buffer after Undo Op = " << past.size());
}

void History::redo(GLfloat imageData[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR])
{
	LOG("Redo");

	if(redoStack.empty())
		return;

	GLfloat **data = redoStack.top();
	redoStack.pop();

	//Have to perform this copy because the stupid glReadPixels function doesn't accept GLfloat** variables, only GLfloat[][]		
	for(int i=0; i<APPLICATION_WINDOW_HEIGHT; i++)
	{		
		for(int j=0; j<APPLICATION_WINDOW_WIDTH * MULT_FACTOR; j++)
		{
			imageData[i][j] = data[i][j];
		}
	}

	past.push_back(data);

	LOG("Current Size of buffer after Redo Op = " << past.size());
}



#endif