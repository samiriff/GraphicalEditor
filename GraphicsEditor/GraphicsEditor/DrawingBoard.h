#ifndef _DRAWINGBOARD_H
#define _DRAWINGBOARD_H
#include "Canvas.h"
#include "ColorPanel.h"
#include "DrawingToolBar.h"
#include "MenuBar.h"
#include "SizeSelector.h"
#include<gl/glut.h>
#include<iostream>
using namespace std;

/*
 *This is the main drawing board class.
 * supposed to contain the object of other classes like toolbar, colorpanel, menubar, and the drawing canvas on which everything is drawn.
 */

class DrawingBoard
{
private:
	Canvas *canvas_board;
	ColorPanel *color_panel;
	MenuBar *menu_bar;
	DrawingToolBar *drawingToolBar;
	SizeSelector *sizeSelector;
public:
	DrawingBoard();
	void initBoard();
	void handleMouseClick(int, int, int, int);
	void handleMouseClickMotion(int, int);
	void handleKeyPress(unsigned char, int, int);
	void handleSpecialKeyPress(int, int, int);
	void handleMousePassiveMotion(int, int);
	~DrawingBoard();
	
	Canvas *getCanvas();
	
};

DrawingBoard::DrawingBoard()
{
	canvas_board = new Canvas(CANVAS_LEFT,CANVAS_BOTTOM, CANVAS_RIGHT,CANVAS_TOP);
	color_panel = new ColorPanel(COLOR_PANEL_LEFT, COLOR_PANEL_BOTTOM, COLOR_PANEL_RIGHT, COLOR_PANEL_TOP);
	menu_bar = new MenuBar(MENUBAR_LEFT, MENUBAR_BOTTOM, MENUBAR_RIGHT, MENUBAR_TOP, canvas_board);
	drawingToolBar = new DrawingToolBar( TOOLBAR_LEFT, TOOLBAR_BOTTOM, TOOLBAR_RIGHT, TOOLBAR_TOP);
	sizeSelector = new SizeSelector(SIZ_SELECTOR_LEFT, SIZ_SELECTOR_BOTTOM, SIZ_SELECTOR_RIGHT, SIZ_SELECTOR_TOP);
}
void DrawingBoard::handleKeyPress(unsigned char c, int x, int y)
{
	switch(c)
	{
	case ',':
		drawingToolBar->IncreasePointSize();
		break;
	case '.':
		drawingToolBar->DecreasePointSize();
		break;	
	}
	
	menu_bar->getTypeWriter()->addChar(c);	
	menu_bar->getTypeWriter()->setCanvasAndTool(canvas_board, drawingToolBar->getSelectedTool());
	glutPostRedisplay();

}

void DrawingBoard::handleSpecialKeyPress(int c, int x, int y)
{
	LOG("DrawingBoard Special Key Function");

	switch(c)
	{
	case GLUT_KEY_F2:			
		canvas_board->restoreOld();
		break;
	case GLUT_KEY_F3:
		canvas_board->restoreNew();
		break;
	case GLUT_KEY_F4:
		color_panel->decreaseTransparencyValue();
		break;
	case GLUT_KEY_F5:
		color_panel->increaseTransparencyValue();
		break;
	}
	glutPostRedisplay();

}

void DrawingBoard::handleMouseClick(int button, int state, int x, int y)
{	
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		LOG("Mouse Click (x,y) = ");
		LOG(x<<' '<<y);
		if(canvas_board->isClickInside(x,y))
		{
			LOG("  Inside CanvasBoard");		
			drawingToolBar->getSelectedTool()->start();
			canvas_board->drawWithTool(drawingToolBar->getSelectedTool(), color_panel->getSelectedColor(), x, y);
		}
		if(color_panel->isClickInside(x,y))
		{
			LOG("  Inside Color Panel");
			color_panel->selectClickedColorFromGrid(x, y);	
			color_panel->selectClickedColorFromTriangle(x, y);	
			if(sizeSelector->isClickInsideSizeSelector(x,y))
				drawingToolBar->setSize(sizeSelector->getSelectedSize());
		}
		if(drawingToolBar->isClickInside(x,y))
		{
			LOG("  Inside Drawing Toolbar");	
			drawingToolBar->selectClickedToolFromGrid(x, y);
			drawingToolBar->setSize(sizeSelector->getSelectedSize());
		}
		if(menu_bar->isClickInsideMenu(x,y))
		{
			LOG("Inside Menu Bar");
			//menu_bar->performMenuOperation(canvas_board);
			menu_bar->selectClickedOpFromGrid(x, y);
		}
	}


	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{		
		color_panel->getSelectedColor().setGLColor();

		if(canvas_board->isClickInside(x, y))
			drawingToolBar->getSelectedTool()->stop();		

		if(canvas_board->hasCanvasChanged())	
		{
			LOG("Canvas Has Changed...Recording");
			canvas_board->recordInHistory();		
		}
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		drawingToolBar->getSelectedTool()->reset();
	}
}

void DrawingBoard::handleMouseClickMotion(int x, int y)
{	
	sizeSelector->updateXYInfo(x,y);
	if(color_panel->isClickInside(x,y))
	{			
		color_panel->selectClickedColorFromTriangle(x, y);			
	}

	if(canvas_board->isClickInside(x,y))
	{
		LOG("  Inside CanvasBoard");		
		canvas_board->drawWithTool(drawingToolBar->getSelectedTool(), color_panel->getSelectedColor(), x, y);
	}
}

void DrawingBoard::handleMousePassiveMotion(int x, int y)
{
	drawingToolBar->selectPassivelyToolFromGrid(x, y);			
	menu_bar->selectPassivelyOpFromGrid(x, y);
	sizeSelector->updateXYInfo(x,y);
}

void DrawingBoard::initBoard()
{
	canvas_board->drawBoard();
	color_panel->drawPanel();
	menu_bar->addMenuBar();
	drawingToolBar->drawToolBar();
	sizeSelector->draw();
}

DrawingBoard::~DrawingBoard()
{
	delete canvas_board;
	delete color_panel;
	delete menu_bar;
	delete drawingToolBar;
}
#endif