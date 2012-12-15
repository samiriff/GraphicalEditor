#ifndef _MENU_OPERATOR_H
#define _MENU_OPERATOR_H

#include <iostream>
#include "Coordinates.h"
#include "Typewriter.h"
#include <gl/glut.h>

using namespace std;

class MenuOp
{
protected:
	Coordinates *bottom_left, *top_right;
	TypeWriter *typeWriter;

	bool isFirstClick;

	void drawText(char *info , float i, float j);

public:
	MenuOp(float x1, float y1, float x2, float y2);
	virtual void render() = 0;
	virtual void performOperation(Canvas *&canvas) = 0;
	void select();
	void deselect();

	//DO NOT FORGET TO SET THE TYPEWRITER BEFORE CALLING performOperation()
	void setTypeWriter(TypeWriter *t);				//Didn't want to change the constructor of each class to incorporate Typewriters..So, I added this method. Sorry for the inconvenience...

	void start();
	void stop();
	
};

MenuOp::MenuOp(float x1, float y1, float x2, float y2)
{
	LOG("MENU OP Class");
	bottom_left = new Coordinates(x1, y1, 0);
	top_right = new Coordinates(x2, y2, 0);	
	isFirstClick = false;
}

void MenuOp::drawText(char *info , float i, float j)
{
	glRasterPos2f(i,j);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*info++);
	}
}

void MenuOp::select()
{
	glColor4f(0, 0, 0, 0.1);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
}

void MenuOp::deselect()
{
	glColor4f(0, 0, 0, 0);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));
}

void MenuOp::setTypeWriter(TypeWriter *t)
{
	typeWriter = t;
}

void MenuOp::start()
{
	isFirstClick = true;
}

void MenuOp::stop()
{
	isFirstClick = false;
}

//******************************************************************************************************************************

//This is a dummy class, selected by default.
class NoOp : public MenuOp
{
public:
	NoOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

NoOp::NoOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void NoOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void NoOp::performOperation(Canvas *&canvas)
{
	LOG("No OP");
	typeWriter->drawString();
}


//******************************************************************************************************************************



class NewOp : public MenuOp
{
public:
	NewOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

NewOp::NewOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void NewOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("New", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void NewOp::performOperation(Canvas *&canvas)
{
	LOG("NEW OP");
	delete canvas;
	canvas = new Canvas(CANVAS_LEFT,CANVAS_BOTTOM, CANVAS_RIGHT,CANVAS_TOP);

	typeWriter->clear();
	typeWriter->setInitialString("A New Beginning");

	deselect();
}



//******************************************************************************************************************************



class OpenOp : public MenuOp
{
private:
	Canvas *canvas;

public:
	OpenOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);

	void continueOperation();
};

OpenOp::OpenOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void OpenOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("Open", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void OpenOp::performOperation(Canvas *&canvas)
{
	LOG("OPEN OP");

	if(isFirstClick)
	{		
		typeWriter->clear();
		typeWriter->setInitialString("Enter Filename: ");
		typeWriter->setSelectedOperation(this);
		typeWriter->start();		
	}
	else
	{
		LOG("I'm here...TypeWriter has buffered input, and I have to process it");
		if(canvas->LoadFromFile(typeWriter->getBufferedInput().c_str()))
		{
			typeWriter->clear();
			typeWriter->setInitialString("Open Successful");
		}
		else
		{
			typeWriter->clear();
			typeWriter->setInitialString("Open UnSuccessful");
		}
	}
}




//******************************************************************************************************************************



class SaveOp : public MenuOp
{
public:
	SaveOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

SaveOp::SaveOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void SaveOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("Save", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void SaveOp::performOperation(Canvas *&canvas)
{
	LOG("SAVE OP");

	if(isFirstClick)
	{		
		typeWriter->clear();
		typeWriter->setInitialString("Enter Filename: ");
		typeWriter->setSelectedOperation(this);
		typeWriter->start();		
	}
	else
	{
		LOG("I'm here...TypeWriter has buffered input, and I have to process it");
		if(canvas->saveToFile(typeWriter->getBufferedInput().c_str()))
		{
			typeWriter->clear();
			typeWriter->setInitialString("Saved Successfully");
		}
		else
		{
			typeWriter->clear();
			typeWriter->setInitialString("BANG! Sorry");
		}
	}
}


//******************************************************************************************************************************



class ExitOp : public MenuOp
{
public:
	ExitOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

ExitOp::ExitOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void ExitOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("Exit", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void ExitOp::performOperation(Canvas *&canvas)
{
	LOG("EXIT OP");

	exit(0);
}

//******************************************************************************************************************************



class CutpasteOp : public MenuOp
{
public:
	CutpasteOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

CutpasteOp::CutpasteOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void CutpasteOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("Cut", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void CutpasteOp::performOperation(Canvas *&canvas)
{
	LOG("CUT-PASTE OP");

	typeWriter->clear();
	typeWriter->setInitialString("Cut and Paste");

	deselect();
}



//******************************************************************************************************************************



class CopypasteOp : public MenuOp
{
public:
	CopypasteOp(float x1, float y1, float x2, float y2);

	void render();
	void performOperation(Canvas *&canvas);
};

CopypasteOp::CopypasteOp(float x1, float y1, float x2, float y2):MenuOp(x1, y1, x2, y2)
{}

void CopypasteOp::render()
{
	glColor3f(0.0,0.0,0.0);
	drawText("Copy", bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS));
}

void CopypasteOp::performOperation(Canvas *&canvas)
{
	LOG("COPY-PASTE OP");

	typeWriter->clear();
	typeWriter->setInitialString("Copy and Paste");

	deselect();
}

#endif