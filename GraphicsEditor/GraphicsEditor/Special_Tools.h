#include "Tool.h"

//The tool.h header file was very big to this one!
class WireCone : public Tool
{
public:
	WireCone(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);

};


WireCone::WireCone(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void WireCone::render()
{
	LOG("Render Wire Cone");
	glColor3f(0.5, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Cone", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void WireCone::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Wire Cone");
	glRasterPos2i(mouseX,mouseY);
	glColor3f(0, 0,0);
	//glutWireCube(10);
	drawText("Not Working", mouseX, mouseY);
}


/*
 *Teapot Tool
 */

class Teapot : public Tool
{
public:
	Teapot(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);

};

Teapot::Teapot(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void Teapot::render()
{
	LOG("Render Teapot");
	glColor3f(1, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 0, 0);
	drawText("T-pot", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
	glColor3f(0, 0, 0);
	glutWireTeapot(4);
}
void Teapot::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw T-bag");
	drawText("Not Working", mouseX, mouseY);
}


class InsideClipper : public Tool
{
public:
	InsideClipper(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};
InsideClipper:: InsideClipper(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void InsideClipper::render()
{
	LOG("Render In Clipper");
	glColor3f(0.5, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("In", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void InsideClipper::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw In Clipper");
	glPointSize(pointSize);
	glLineWidth(pointSize);
	if(isFirstPointSelected)
	{		
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{			
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		glColor3f(1,1,1);
		//Clip the right part
		glBegin(GL_POLYGON);
		glVertex2i(mouseX, mouseY);
		glVertex2i(mouseX,firstPoint.get(Y_AXIS));
		glVertex2i(CANVAS_RIGHT, CANVAS_TOP);
		glVertex2i(CANVAS_RIGHT, CANVAS_BOTTOM);
		glEnd();
		
		//Clip the left part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glVertex2i(firstPoint.get(X_AXIS),mouseY);
		glVertex2i(firstPoint.get(X_AXIS),firstPoint.get(Y_AXIS));
		glVertex2i(CANVAS_LEFT, CANVAS_TOP);
		glEnd();

		//clip the top part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_TOP);
		glVertex2i(firstPoint.get(X_AXIS),firstPoint.get(Y_AXIS));
		glVertex2i(mouseX,firstPoint.get(Y_AXIS));
		glVertex2i(CANVAS_RIGHT, CANVAS_TOP);
		glEnd();
		
		//clip the bottom part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glVertex2i(firstPoint.get(X_AXIS),mouseY);
		glVertex2i(mouseX, mouseY);
		glVertex2i(CANVAS_RIGHT, CANVAS_BOTTOM);
		glEnd();
	}	
}
class OutClipper : public Tool
{
public:
	OutClipper(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};

OutClipper:: OutClipper(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void OutClipper::render()
{
	LOG("Render OutClipper");
	glColor3f(0.5, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Out", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void OutClipper::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw OutClipper");
	glPointSize(pointSize);
	glLineWidth(pointSize);
	if(isFirstPointSelected)
	{		
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{			
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2i(mouseX, mouseY);
		glVertex2i(mouseX,firstPoint.get(Y_AXIS));
		glVertex2i(firstPoint.get(X_AXIS),firstPoint.get(Y_AXIS));
		glVertex2i(firstPoint.get(X_AXIS),mouseY);
		glEnd();
	}	
}
class Translate : public Tool
{
public:
	Translate(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	


};

Translate::Translate(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void Translate::render()
{
	LOG("Render Translate");
	glColor3f(1, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 0, 0);
	drawText("Tra", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
	glColor3f(0, 0, 0);
	glutWireTeapot(4);
}
void Translate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Translate");
	drawText("Translate", mouseX, mouseY);
}
class Scale : public Tool
{
public:
	Scale(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};
Scale::Scale(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void Scale::render()
{
	LOG("Render Scale");
	glColor3f(1, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 0, 0);
	drawText("Scale", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void Scale::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Scale");
	drawText("Scale!", mouseX, mouseY);
}

class Rotate : public Tool
{
public:
	Rotate(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

Rotate::Rotate(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void Rotate::render()
{
	LOG("Render Rotate");
	glColor3f(1, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Rot", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void Rotate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Rotate");
	drawText("Rotate!", mouseX, mouseY);
}

class FloodFiller : public Tool
{
public:
	FloodFiller(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};
FloodFiller::FloodFiller(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void FloodFiller::render()
{
	LOG("Render FloodFiller");
	glColor3f(1, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 0, 0);
	drawText("Fill", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 20, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}
void FloodFiller::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw FloodFiller");
	drawText("Fill it!", mouseX, mouseY);
}