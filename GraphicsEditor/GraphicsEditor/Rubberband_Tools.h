#include <cmath>
#include <vector>
#include "Tool.h"
#include "Color.h"

using namespace std;

class Line : public Tool
{
protected:
	Coordinates previous;	
	Color selectedColor;

	void drawLine(float x1, float y1, float x2, float y2);
	void rubberband(int mouseX, int mouseY, bool &checkingConditionForPolygon);

public:
	Line(float x1, float y1, float x2, float y2);
	
	void stop();

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);		
};


Line::Line(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Line::drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();	
}

void Line::render()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), top_right->get(Y_AXIS));
		glVertex2f(top_right->get(X_AXIS), bottom_left->get(Y_AXIS));
	glEnd();

	glColor3f(0, 0, 0);
	drawText("Line", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Line::rubberband(int mouseX, int mouseY, bool &checkingConditionForPolygon)
{
	if(isFirstPointSelected && checkingConditionForPolygon == false)
	{
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);
				
		isFirstPointSelected = false;
		previous = firstPoint;

		//Tracking the color
		Tool::getCurrentColor(selectedColor);	
	}
	else
	{	
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);

		//Removing old line by xoring again				
		selectedColor.setInvertedGLColor();		
		drawLine(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));			
		glFlush();

		//Drawing new line
		drawLine(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), mouseX, mouseY);					
		glFlush();
		
		glDisable(GL_COLOR_LOGIC_OP);		

		previous.set(X_AXIS, mouseX);
		previous.set(Y_AXIS, mouseY);
	}
}

void Line::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{	
	glLineWidth(pointSize);

	this->canvas = canvas;

	bool dummyVariable = false;
	rubberband(mouseX, mouseY, dummyVariable);
}

void Line::stop()
{
	if(canvas == NULL)
		return;

	drawLine(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));			
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	

	canvas = NULL;			//To reset
}

//**********************************************************************************************************************//


class Polygon : public Line
{
protected:
	Coordinates startingPoint;		
	bool isChoosing;		

public:
	Polygon(float x1, float y1, float x2, float y2);
		
	void stop();
	void reset();

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);		
};

Polygon::Polygon(float x1, float y1, float x2, float y2):Line(x1, y1, x2, y2)
{
	isChoosing = false;
}

void Polygon::render()
{
	glColor3f(1, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);

	glColor3f(1, 1, 1);
	drawText("Polygon", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 7, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Polygon::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{	
	LOG("DRAWING ON CANVAS");

	glLineWidth(pointSize);
	this->canvas = canvas;

	bool doIhaveToChoose = false;
	if(isFirstPointSelected && !isChoosing)
		doIhaveToChoose = true;
	
	rubberband(mouseX, mouseY, isChoosing);

	if(doIhaveToChoose)
	{
		LOG("I HAVE TO CHOOSE");
		isChoosing = false;
		startingPoint = firstPoint;
	}	
}

void Polygon::stop()
{
	if(canvas == NULL)
		return;

	drawLine(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));			
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	
	
	isChoosing = true;
	firstPoint = previous;
}

void Polygon::reset()
{
	LOG("RESET");

	if(canvas == NULL)
		return;

	isChoosing = false;
	selectedColor.setGLColor();
	drawLine(startingPoint.get(X_AXIS), startingPoint.get(Y_AXIS), firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS));	
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	

	canvas = NULL;
}


//**********************************************************************************************************************//

class FilledPolygon : public Polygon
{
private:
	vector<Coordinates> pointsQueue;

	void drawFilledPolygon();

public:
	FilledPolygon(float x1, float y1, float x2, float y2);

	void render();

	void stop();
	void reset();
};

FilledPolygon::FilledPolygon(float x1, float y1, float x2, float y2):Polygon(x1, y1, x2, y2)
{
	pointsQueue.clear();
}

void FilledPolygon::render()
{
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);

	glColor3f(1, 0, 0);
	drawText("FPolygon", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 9, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void FilledPolygon::stop()
{
	Polygon::stop();
		
	LOG("FILLED POLYGON STOP");
	LOG("FIRST POINT = " << firstPoint);
	pointsQueue.push_back(firstPoint);	
}

void FilledPolygon::drawFilledPolygon()
{
	LOG("NUMBER OF POINTS = " << pointsQueue.size());
	glBegin(GL_POLYGON);
	for(int i=0; i<pointsQueue.size(); i++)
		glVertex2f(pointsQueue[i].get(X_AXIS), pointsQueue[i].get(Y_AXIS));
	glEnd();
}

void FilledPolygon::reset()
{
	LOG("RESET");

	if(canvas == NULL)
		return;

	isChoosing = false;
	pointsQueue.insert(pointsQueue.begin(), startingPoint);

	selectedColor.setGLColor();
	drawFilledPolygon();	
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	

	pointsQueue.clear();
	canvas = NULL;
}

//**********************************************************************************************************************//

class Rect : public Tool
{
private:
	Coordinates previous;	
	Color selectedColor;

public:
	Rect(float x1, float y1, float x2, float y2);

	void stop();

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

Rect::Rect(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Rect::render()
{	
	glColor3f(1, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Rect", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Rect::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{	
	glPointSize(pointSize);
	this->canvas = canvas;

	if(isFirstPointSelected)
	{
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);
		
		isFirstPointSelected = false;
		previous = firstPoint;

		//Tracking the color
		Tool::getCurrentColor(selectedColor);		
	}
	else
	{	
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);

		//Removing old rectangle by xoring again				
		selectedColor.setInvertedGLColor();		
		glRectf(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
		glFlush();

		//Drawing new rectangle
		glRectf(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), mouseX, mouseY);		
		glFlush();
		
		glDisable(GL_COLOR_LOGIC_OP);		

		previous.set(X_AXIS, mouseX);
		previous.set(Y_AXIS, mouseY);
	}
}

void Rect::stop()
{
	if(canvas == NULL)
		return;

	glRectf(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	

	canvas = NULL;			//To reset
}

//**********************************************************************************************************************//

class UnfilledRect : public Tool
{
private:
	Coordinates previous;	
	Color selectedColor;

	void drawLineLoop(float x1, float y1, float x2, float y2);

public:
	UnfilledRect(float x1, float y1, float x2, float y2);

	void stop();

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};


UnfilledRect::UnfilledRect(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}

void UnfilledRect::drawLineLoop(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);			
	glEnd();	
}

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
	this->canvas = canvas;

	if(isFirstPointSelected)
	{
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);
		
		isFirstPointSelected = false;
		previous = firstPoint;

		//Tracking the color
		Tool::getCurrentColor(selectedColor);		
	}
	else
	{	
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);

		//Removing old rectangle by xoring again				
		selectedColor.setInvertedGLColor();		
		drawLineLoop(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
		glFlush();

		//Drawing new rectangle
		drawLineLoop(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), mouseX, mouseY);		
		glFlush();
		
		glDisable(GL_COLOR_LOGIC_OP);		

		previous.set(X_AXIS, mouseX);
		previous.set(Y_AXIS, mouseY);
	}
}

void UnfilledRect::stop()
{
	if(canvas == NULL)
		return;

	drawLineLoop(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);	

	canvas = NULL;			//To reset
}

//**********************************************************************************************************************//

class Circle : public Tool
{
private:
	Coordinates previous;	
	Color selectedColor;

	void drawCircle(float x1, float y1, float x2, float y2);

public:
	Circle(float x1, float y1, float x2, float y2);

	void stop();

	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

Circle::Circle(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void Circle::drawCircle(float x1, float y1, float x2, float y2)
{
	float dx = (x1 - x2);
	float dy = (y1 - y2);
	float theta = 0;
	float xc = (x1 + x2) / 2, yc = (y1 + y2) / 2; 
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
	glVertex2f(x+xc,y+yc);			
	glEnd();
}

void Circle::render()
{	
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Circle", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 6, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Circle::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	this->canvas = canvas;

	if(isFirstPointSelected)
	{
		firstPoint.set(X_AXIS, mouseX);
		firstPoint.set(Y_AXIS, mouseY);
		
		isFirstPointSelected = false;
		previous = firstPoint;

		//Tracking the color
		Tool::getCurrentColor(selectedColor);		
	}
	else
	{	
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);

		//Removing old circle by xoring again				
		selectedColor.setInvertedGLColor();		
		drawCircle(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
		glFlush();

		//Drawing new circle
		drawCircle(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), mouseX, mouseY);		
		glFlush();
		
		glDisable(GL_COLOR_LOGIC_OP);		

		previous.set(X_AXIS, mouseX);
		previous.set(Y_AXIS, mouseY);
	}
}

void Circle::stop()
{
	if(canvas == NULL)
		return;

	drawCircle(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS), previous.get(X_AXIS), previous.get(Y_AXIS));				
	glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, canvas->imageData);

	canvas = NULL;			//To reset
}


