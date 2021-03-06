#ifndef _DRAWING_TOOL_BAR_H
#define _DRAWING_TOOL_BAR_H

#include "Coordinates.h"
#include "Constants.h"
#include "Tool.h"
#include "Special_Tools.h"
#include "Rubberband_Tools.h"
#include<gl/glut.h>



class DrawingToolBar
{
private:
	Coordinates *bottom_left, *top_right;
	Tool *tools[TOOLBAR_NUM_ROWS][TOOLBAR_NUM_COLS];
	Tool *selectedTool;
	Tool *selectedPassiveTool;

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
	void selectPassivelyToolFromGrid(int x, int y);
	void setSize(Size);
	Tool *getSelectedTool();
};

bool DrawingToolBar::isClickInside(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		return (y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS));
	else
		return false;
}
void DrawingToolBar::setSize(Size size)
{
	selectedTool->setDrawingSize(size);
}
DrawingToolBar::DrawingToolBar(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);

	initTools();
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

	tools[0][3] = new PaintBrush(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[0][4] = new Polygon(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[0][5] = new UnfilledRect(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[0][6] = new RingDrawer(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[0][7] = new FloodFiller(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[0][8] = new OutClipper(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	

	begin_x = bottom_left->get(X_AXIS)+GAP;
	begin_y -= (TOOL_BORDER_HEIGHT+GAP);

	tools[1][0] = new Rect(begin_x, begin_y-TOOL_BORDER_HEIGHT,begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	
	tools[1][1] = new Circle(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;
	
	tools[1][2] = new Eraser(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][3] = new Text(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][4] = new FilledPolygon(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][5] = new Scale(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][6] = new Translate(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	Translate *translateTool = (Translate *)tools[1][6];
	translateTool->convertToCutOperator();	
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][7] = new Translate(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;

	tools[1][8] = new InsideClipper(begin_x,begin_y-TOOL_BORDER_HEIGHT, begin_x+TOOL_BORDER_WIDTH, begin_y);
	begin_x += TOOL_BORDER_WIDTH+GAP;




	selectedTool = tools[0][0];
	selectedPassiveTool = tools[0][0];
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
	selectedTool->drawPointSizeInfo();

	if(selectedPassiveTool != NULL)
		selectedPassiveTool->select();
	
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
		LOG("Row = " << row << "\tCol = " << col);	
		selectedTool = tools[row][col];		
	}	
}

void DrawingToolBar::selectPassivelyToolFromGrid(int x, int y)
{
	if(isClickInsideGrid(x, y))
	{
		int row = (top_right->get(Y_AXIS) - y) / TOOL_BORDER_HEIGHT;
		int col = (x - bottom_left->get(X_AXIS)) / (TOOL_BORDER_WIDTH + GAP);
		LOG("Row = " << row << "\tCol = " << col);	
		selectedPassiveTool = tools[row][col];		
	}	
	else
	{
		selectedPassiveTool = NULL;
	}
}

Tool *DrawingToolBar::getSelectedTool()
{
	return selectedTool;
}


#endif