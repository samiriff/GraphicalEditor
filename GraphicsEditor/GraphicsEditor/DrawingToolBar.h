#ifndef _DRAWING_TOOL_BAR_H
#define _DRAWING_TOOL_BAR_H

#include "Coordinates.h"
#include "Constants.h"
#include "Tool.h"
#include<gl/glut.h>



class DrawingToolBar
{
private:
	Coordinates *bottom_left, *top_right;
	Tool *tools[TOOLBAR_NUM_ROWS][TOOLBAR_NUM_COLS];
	Tool *selectedTool;

	void initTools();
	void drawTools();
public:
	DrawingToolBar(float, float , float , float);
	void drawToolBar();
	void IncreasePointSize();
	void DecreasePointSize();
	bool isClickInside(int , int);
	bool isClickInsideGrid(int x, int y);
	void selectClickedToolFromGrid(int x, int y);

	Tool *getSelectedTool();
};

bool DrawingToolBar::isClickInside(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		return (y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS));
	else
		return false;
}

DrawingToolBar::DrawingToolBar(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);

	initTools();
	selectedTool->IncreasePointSize();
}

void DrawingToolBar::initTools()
{
	float begin_x = bottom_left->get(X_AXIS)+GAP, begin_y = top_right->get(Y_AXIS)-GAP;

	tools[0][0] = new Pencil(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	tools[0][1] = new Spray(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	tools[0][2] = new Line(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	begin_x = bottom_left->get(X_AXIS)+GAP;
	begin_y -= (TOOL_BORDER_HEIGHT+GAP);

	tools[1][0] = new Rect(begin_x, begin_y-TOOL_BORDER_HEIGHT,begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	tools[1][1] = new Circle(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	tools[1][2] = new Eraser(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;


	selectedTool = tools[0][0];
}

void DrawingToolBar::drawToolBar()
{
	glColor3f(1,1,1);
	//glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glBegin(GL_LINE_LOOP);
		glVertex2f(bottom_left->get(X_AXIS),bottom_left->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS),bottom_left->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS),top_right->get(Y_AXIS));
		glVertex2f(bottom_left->get(X_AXIS),top_right->get(Y_AXIS));
	glEnd();
	drawTools();
}

void DrawingToolBar::drawTools()
{	
	for(int i=0; i<TOOLBAR_NUM_ROWS; i++)
	{
		for(int j=0; j<TOOLBAR_NUM_COLS; j++)
		{
			tools[i][j]->render();
		}
	}
	
	selectedTool->select();		
}
void DrawingToolBar::IncreasePointSize()
{
	if(selectedTool)
	{
		selectedTool->IncreasePointSize();
	}
}
void DrawingToolBar::DecreasePointSize()
{
	if(selectedTool)
	{
		selectedTool->DecreasePointSize();
	}
}

bool DrawingToolBar::isClickInsideGrid(int x, int y)
{
	if((x >= bottom_left->get(X_AXIS))&&(x <= bottom_left->get(X_AXIS) + (TOOL_BORDER_WIDTH + GAP) * TOOLBAR_NUM_COLS))
		return (y < top_right->get(Y_AXIS))&&(y > top_right->get(Y_AXIS) - TOOL_BORDER_HEIGHT * TOOLBAR_NUM_ROWS);
	else
		return false;	
}

void DrawingToolBar::selectClickedToolFromGrid(int x, int y)
{	
	if(isClickInsideGrid(x, y))
	{
		int row = (top_right->get(Y_AXIS) - y) / TOOL_BORDER_HEIGHT;
		int col = (x - bottom_left->get(X_AXIS)) / (TOOL_BORDER_WIDTH + GAP);
		cout << "Row = " << row << "\tCol = " << col << endl;		
		selectedTool = tools[row][col];		
	}	
}

Tool *DrawingToolBar::getSelectedTool()
{
	return selectedTool;
}


#endif