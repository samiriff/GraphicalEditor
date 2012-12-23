#ifndef _MENU_BAR_H
#define _MENU_BAR_H


#include "Coordinates.h"
#include "Constants.h"
#include "MenuOperator.h"
#include "Typewriter.h"
#include<gl/glut.h>


class MenuBar
{
private:
	Coordinates *bottom_left, *top_right;
	TypeWriter *typeWriter;

	MenuOp *menuOps[MENUBAR_NUM_ROWS][MENUBAR_NUM_COLS];
	MenuOp *selectedOp;
	Canvas *canvas;

	MenuOp *selectedPassiveOp;

	void initMenuOptions();
	void drawMenuOptions();
	void drawText(char *info, float x, float y);
	void addMenuItems();
	MenuOps selectedMenuOperation;
	bool isCutPasteSelected;
	bool isCopyPasteSelected;
	void updateSelectionBox();

	void select();
public:
	MenuBar( float,float,float,float, Canvas *);
	void addMenuBar();
	bool isClickInsideMenu(int , int );
	MenuOps getSelectedMenuOp(int, int);
	void performMenuOperation(Canvas *);

	void selectClickedOpFromGrid(int x, int y);
	void selectPassivelyOpFromGrid(int x, int y);

	TypeWriter *getTypeWriter();
	void drawString(float x, float y, string buffer);
};

MenuBar::MenuBar(float x1, float y1, float x2, float y2, Canvas *c)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
	isCutPasteSelected = isCopyPasteSelected = false;

	canvas = c;

	initMenuOptions();
}

void MenuBar::initMenuOptions()
{
	float begin_x = OP_NEW_X, begin_y = OP_Y;

	menuOps[0][0] = new NewOp(begin_x, begin_y, begin_x + MENUBAR_BORDER_WIDTH, top_right->get(Y_AXIS));
	begin_x += MENUBAR_BORDER_WIDTH;
	menuOps[0][1] = new OpenOp(begin_x, begin_y, begin_x + MENUBAR_BORDER_WIDTH, top_right->get(Y_AXIS));
	begin_x += MENUBAR_BORDER_WIDTH;
	menuOps[0][2] = new SaveOp(begin_x, begin_y, begin_x + MENUBAR_BORDER_WIDTH, top_right->get(Y_AXIS));
	begin_x += MENUBAR_BORDER_WIDTH;
	menuOps[0][3] = new ExitOp(begin_x, begin_y, begin_x + MENUBAR_BORDER_WIDTH, top_right->get(Y_AXIS));
	begin_x += MENUBAR_BORDER_WIDTH;
	menuOps[0][4] = new TextOp(begin_x, begin_y, begin_x + MENUBAR_BORDER_WIDTH, top_right->get(Y_AXIS));
	begin_x += MENUBAR_BORDER_WIDTH;

	//This is always the last operator - No Op
	menuOps[0][5] = new NoOp(0, 0, 0, 0);		
	begin_x += MENUBAR_BORDER_WIDTH;

	typeWriter = new TypeWriter(begin_x, begin_y, "Status:");


	selectedOp = menuOps[0][MENUBAR_NUM_COLS - 1];
	selectedPassiveOp = menuOps[0][MENUBAR_NUM_COLS - 1];
}

void MenuBar::drawMenuOptions()
{
	for(int i=0; i<MENUBAR_NUM_ROWS; i++)
	{
		for(int j=0; j<MENUBAR_NUM_COLS; j++)
		{
			menuOps[i][j]->render();
		}
	}	

	if(typeWriter->isBusy())
	{
		selectedOp = typeWriter->getSelectedOperation();
		selectedOp->select();
		LOG("Busy Accepting Input From User...Do not check for any other menu events");
		if(typeWriter->isFinished())
		{
			LOG("Completed Input...User has pressed the Enter key...Now it is time....");			
			selectedOp->stop();
			glutPostRedisplay();
			selectedOp->performOperation(canvas);

			typeWriter->setSelectedOperation(NULL);
			typeWriter->stop();

			selectedOp = menuOps[0][MENUBAR_NUM_COLS - 1];				//Go back to No Op
		}		
	}
	else
	{
		LOG("TypeWriter is not in use...Service this request and move on");
		selectedOp->select();
		selectedOp->setTypeWriter(typeWriter);
		selectedOp->start();	
		selectedOp->performOperation(canvas);	

		selectedOp = menuOps[0][MENUBAR_NUM_COLS - 1];				//Go back to No Op

		if(selectedPassiveOp != NULL)
			selectedPassiveOp->select();
	}	

	typeWriter->drawString();	
}

void MenuBar::addMenuItems()
{
	glColor3f(0.0,0.0,0.0);	
	drawMenuOptions();	
}

bool MenuBar::isClickInsideMenu(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x - 10 <= bottom_left->get(X_AXIS) + (MENUBAR_BORDER_WIDTH) * MENUBAR_NUM_COLS))
		if((y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS)))					
			return true;		
	else
		return false;

}

void MenuBar::selectClickedOpFromGrid(int x, int y)
{	
	if(isClickInsideMenu(x, y))
	{
		int row = (top_right->get(Y_AXIS) - y) / MENUBAR_BORDER_HEIGHT;
		int col = (x - bottom_left->get(X_AXIS)) / (MENUBAR_BORDER_WIDTH);

		if(col >= MENUBAR_NUM_COLS)					//Added this because isClickInsideMenu() isn't working properly
			return;

		cout << "Row = " << row << "\tCol = " << col << endl;		
		selectedOp = menuOps[row][col];		
	}	
}

void MenuBar::selectPassivelyOpFromGrid(int x, int y)
{
	if(isClickInsideMenu(x, y))
	{	
		int row = (top_right->get(Y_AXIS) - y) / MENUBAR_BORDER_HEIGHT;
		int col = (x - bottom_left->get(X_AXIS)) / (MENUBAR_BORDER_WIDTH);

		if(col >= MENUBAR_NUM_COLS)					//Added this because isClickInsideMenu() isn't working properly
			return;
		if(row >= MENUBAR_NUM_ROWS)
			return;


		LOG("Row = " << row << "\tCol = " << col);
		selectedPassiveOp = menuOps[row][col];		
	}	
	else
	{
		selectedPassiveOp = NULL;
	}
}

void MenuBar::addMenuBar()
{
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(bottom_left->get(X_AXIS),bottom_left->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS),bottom_left->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS),top_right->get(Y_AXIS));
		glVertex2f(bottom_left->get(X_AXIS),top_right->get(Y_AXIS));
	glEnd();
	addMenuItems();

	//Repeated for BOLD effect
	drawString(top_right->get(X_AXIS) - 137, top_right->get(Y_AXIS) - 15, "F2/F3 = Undo/Redo");	
	drawString(top_right->get(X_AXIS) - 137, top_right->get(Y_AXIS) - 16, "F2/F3 = Undo/Redo");	
	drawString(top_right->get(X_AXIS) - 136, top_right->get(Y_AXIS) - 16, "F2/F3 = Undo/Redo");	
	drawString(top_right->get(X_AXIS) - 137, top_right->get(Y_AXIS) - 30, "F4/F5 = +/- Alpha");
	drawString(top_right->get(X_AXIS) - 137, top_right->get(Y_AXIS) - 31, "F4/F5 = +/- Alpha");
	drawString(top_right->get(X_AXIS) - 136, top_right->get(Y_AXIS) - 31, "F4/F5 = +/- Alpha");
}

void MenuBar::drawString(float x, float y, string buffer)
{		
	glColor4f(1, 0, 0.5, 1);
	
	glRasterPos2f(x, y);
	for(int i=0; i<buffer.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
}

TypeWriter *MenuBar::getTypeWriter()
{
	return typeWriter;
}


#endif