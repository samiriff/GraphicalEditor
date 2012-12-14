#include "Coordinates.h"
#include "Constants.h"
#include<gl/glut.h>


class MenuBar
{
private:
		Coordinates *bottom_left, *top_right;
		void initMenuOptions();
		void drawText(char *info, float x, float y);
		void addMenuItems();
		MenuOps selectedMenuOperation;
		bool isCutPasteSelected;
		bool isCopyPasteSelected;
		void updateSelectionBox();
public:
	MenuBar( float,float,float,float);
	void addMenuBar();
	bool isClickInsideMenu(int , int );
	MenuOps getSelectedMenuOp(int, int);
	void performMenuOperation(Canvas *);

};
MenuBar::MenuBar(float x1, float y1, float x2, float y2)
{
	bottom_left = new Coordinates(x1,y1,0);
	top_right = new Coordinates(x2,y2,0);
	isCutPasteSelected = isCopyPasteSelected = false;
}
void MenuBar::updateSelectionBox()
{
	if(isCutPasteSelected)
	{

	}
	if(isCopyPasteSelected)
	{

	}

}
bool MenuBar::isClickInsideMenu(int x, int y)
{
	if((x>=bottom_left->get(X_AXIS))&&(x<=top_right->get(X_AXIS)))
		if((y>=bottom_left->get(Y_AXIS))&&(y<=top_right->get(Y_AXIS)))
		{
			selectedMenuOperation = getSelectedMenuOp(x,y);
			return true;
		}
	else
		return false;

}
void MenuBar::performMenuOperation(Canvas * canvas_board)
{
	switch(selectedMenuOperation)
	{
	case NEW:
		delete canvas_board;
		canvas_board = new Canvas(CANVAS_LEFT,CANVAS_BOTTOM, CANVAS_RIGHT,CANVAS_TOP);
			break;
	case OPEN:
		canvas_board->LoadFromFile();
		break;
	case SAVE:
		canvas_board->saveToFile();
		break;
	case EXIT: exit(0);		
		break;
	case CUT_PASTE:
		isCutPasteSelected = (isCutPasteSelected) ? false:true;
		updateSelectionBox();
		break;
	case COPY_PASTE:
		isCopyPasteSelected = isCopyPasteSelected?false:true;
		updateSelectionBox();
		break;
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
}
void MenuBar::drawText(char *info , float i, float j)
{
	glRasterPos2f(i,j);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*info++);
	}
}
void MenuBar::addMenuItems()
{
	glColor3f(0.0,0.0,0.0);
	drawText("New",OP_NEW_X,top_right->get(Y_AXIS)-20);
    drawText("Open",OP_OPEN_X,top_right->get(Y_AXIS)-20);
    drawText("Save",OP_SAVE_X,top_right->get(Y_AXIS)-20);
    drawText("Exit",OP_EXIT_X,top_right->get(Y_AXIS)-20);
    drawText("Cut/Paste",OP_CUPASTE_X,top_right->get(Y_AXIS)-20);
	drawText("Copy/Paste",OP_COPY_X,top_right->get(Y_AXIS)-20);
}
MenuOps MenuBar::getSelectedMenuOp(int x, int y)
{
	if(x>=OP_NEW_X && x<=OP_OPEN_X)
	{
		LOG("NEW OPERATION")
		return NEW;
	}
	if(x>=OP_OPEN_X && x<=OP_SAVE_X)
	{
		LOG("OPEN OPERATION")
		return OPEN;
	}
	if(x>=OP_SAVE_X && x<=OP_EXIT_X)
	{
		LOG("SAVE OPERATION")
		return SAVE;
	}
	if(x>=OP_EXIT_X && x<=OP_CUPASTE_X)
	{
		LOG("EXIT OPERATION")
		return EXIT;
	}
	if(x>=OP_CUPASTE_X && x<=OP_COPY_X)
	{
		LOG("CUT/PASTE OPERATION")
		return CUT_PASTE;
	}
	if(x>=OP_COPY_X && x<= OP_COPY_X+70)
	{
		LOG("COPY/PASTE OPERATION")
		return COPY_PASTE;
	}
	return EXIT;
}