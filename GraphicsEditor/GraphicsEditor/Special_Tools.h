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

//class Translate : public Tool
//{
//public:
//	Translate(float x1, float y1, float x2, float y2);
//	void render();
//	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
//
//
//};
//class Scale : public Tool
//{
//public:
//	Scale(float x1, float y1, float x2, float y2);
//	void render();
//	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
//};
//class Rotate : public Tool
//{
//public:
//	Rotate(float x1, float y1, float x2, float y2);
//	void render();
//	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
//};