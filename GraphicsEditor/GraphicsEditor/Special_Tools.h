#include "Tool.h"
#include "Color.h"
//The tool.h header file was very big to this one!
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
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Translate", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 9, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
	glColor3f(0, 0, 0);
	glutWireTeapot(4);
}

void Translate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Translate");
	drawText("Translate", mouseX, mouseY);
}

//**********************************************************************************************************************//

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
	glColor3f(0, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Scale", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 5, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}
void Scale::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Scale");
	drawText("Scale!", mouseX, mouseY);
}

//**********************************************************************************************************************//

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
	drawText("Rotate", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}
void Rotate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Rotate");
	drawText("Rotate!", mouseX, mouseY);
}


//**********************************************************************************************************************//

class FloodFiller : public Tool
{
public:
	FloodFiller(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
	void Fill(int, int, Color &, int depth);
};
FloodFiller::FloodFiller(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void FloodFiller::render()
{
	LOG("Render FloodFiller");
	glColor3f(0, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Fill", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}
void FloodFiller::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw FloodFiller");
	GLfloat rgbValues[3];	
	glReadPixels(mouseX, mouseY, 1, 1, GL_RGB, GL_FLOAT, rgbValues);
	Color currentColor(rgbValues[0],rgbValues[1],rgbValues[2]);
	LOG("Current Colour = " << currentColor);	
	glPointSize(1);
	Fill(mouseX, mouseY, currentColor, 200);
}
void FloodFiller::Fill(int x, int y, Color &previousPixelColor, int depth)
{	
	if(depth <=0 ) 
		return;

	GLfloat rgbValues[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, rgbValues);
	Color currentColor(rgbValues[0],rgbValues[1],rgbValues[2]);

	if (currentColor == previousPixelColor)
	{
		depth -= 1;		
		glBegin(GL_POINTS); 
			glVertex2i(x,y); 
		glEnd();
		Fill(x-1, y, currentColor,depth);
		Fill(x+1, y, currentColor,depth);
		Fill(x, y-1, currentColor,depth);
		Fill(x, y+1, currentColor,depth);
	}
	return;
}

//**********************************************************************************************************************//

class UnfilledRect : public Tool
{
public:
	UnfilledRect(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);

};


UnfilledRect::UnfilledRect(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void UnfilledRect::render()
{
	LOG("Render UnfilledRect");
	glColor3f(0.5, 0, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Loop", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}
void UnfilledRect::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw UnfilledRect");
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


		glBegin(GL_LINE_LOOP);
			glVertex2f(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
			glVertex2f(firstPoint.get(X_AXIS), mouseY);
			glVertex2f(mouseX, mouseY);
			glVertex2f(mouseX, firstPoint.get(Y_AXIS));			
		glEnd();		
	}	
	
}

//**********************************************************************************************************************//

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
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("T-pot", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 5, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
	glColor3f(0, 0, 0);
	glutWireTeapot(4);
}
void Teapot::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw T-bag");
	drawText("Not Working", mouseX, mouseY);
}

//**********************************************************************************************************************//


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
	glColor3f(1, 1, 1);
	drawText("Clip-In", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 7, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
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

//**********************************************************************************************************************//

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
	glColor3f(1, 1, 1);
	drawText("Clip-Out", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 9, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
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
class RingDrawer : public Tool
{
public:
	RingDrawer(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};

//**********************************************************************************************************************//

RingDrawer::RingDrawer(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void RingDrawer::render()
{	
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Ring", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - 16, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - 8);
}

void RingDrawer::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	if(isFirstPointSelected)
	{		
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);

		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{			
		//glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		//glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		float dx = (firstPoint.get(X_AXIS) - mouseX);
		float dy = (firstPoint.get(Y_AXIS) - mouseY);
		float theta = 0;
		float xc = (firstPoint.get(X_AXIS)+mouseX)/2, yc = (firstPoint.get(Y_AXIS) + mouseY)/2; 
		float r = sqrt(dx*dx + dy*dy)/2;
		float x=0,y=0;
		glBegin(GL_LINE_STRIP);
		while(theta <= 360)
		{
			x = r*cos(theta);
			y = r*sin(theta);			
			glVertex2f(x+xc,y+yc);			
			theta += 0.25;
		}
		glEnd();	
	}	
}


//**********************************************************************************************************************//