#include <cmath>

#include "Tool.h"
#include "Color.h"
//The tool.h header file was very big to this one!
class Translate : public Tool
{
protected:
	Coordinates secondPoint;
	bool isSecondPointSelected;
	Coordinates thirdPoint;
	bool isThirdPointSelected;
	Coordinates fourthPoint;
	bool isfourthPointSelected;

	bool inMotion;

	bool eraseSelection;

	void fourthPointSelection(int mouseX, int mouseY);
public:
	Translate(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	

	void stop();
	void convertToCutOperator();
};

Translate::Translate(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	isfourthPointSelected = isSecondPointSelected = isThirdPointSelected = false;
	inMotion = false;

	eraseSelection = false;
}

void Translate::stop()
{	
	inMotion = false;
	
	LOG("Motion Switched Off - STOP Function");			
}

void Translate::convertToCutOperator()
{
	eraseSelection = true;
}

void Translate::render()
{
	LOG("Render Translate");
	glColor3f(1, 1, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);

	if(eraseSelection)
		drawText("Cut", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 3, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
	else
		drawText("Copy", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
	glColor3f(0, 0, 0);	
}

void Translate::fourthPointSelection(int mouseX, int mouseY)
{
	if(!isfourthPointSelected)
	{
		LOG("Before motion check -- Fourth Point Selected");	
		if(inMotion)
		{
			LOG("Fourth Point Selected");		
		
			secondPoint.setToBoundingBoxCoordinates(thirdPoint);	

			glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
			glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		
			LOG("Bounding Box width = " << (thirdPoint.get(X_AXIS) - secondPoint.get(X_AXIS)) / 2.0);
			LOG("Mouse X = " << mouseX << "\tMouseY = " << mouseY);

			fourthPoint.set(X_AXIS, mouseX - (thirdPoint.get(X_AXIS) - secondPoint.get(X_AXIS)) / 2.0);
			fourthPoint.set(Y_AXIS, mouseY - (thirdPoint.get(Y_AXIS) - secondPoint.get(Y_AXIS)) / 2.0);

			LOG("Fourth Point After = " << fourthPoint);

			//Copies the contents of the bounding box of the selection to another part of the canvas, the bounding box of which has its bottom left coordinates at fourthPoint
			glRasterPos2f(fourthPoint.get(X_AXIS), fourthPoint.get(Y_AXIS));
			glCopyPixels(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS), thirdPoint.get(X_AXIS) - secondPoint.get(X_AXIS) - 1, thirdPoint.get(Y_AXIS) - secondPoint.get(Y_AXIS) - 1, GL_COLOR);
		

			if(eraseSelection)
			{
				//Cut operation
				glColor3f(1, 1, 1);
				glRectf(secondPoint.get(X_AXIS) - 1, secondPoint.get(Y_AXIS) - 1, thirdPoint.get(X_AXIS), thirdPoint.get(Y_AXIS));	//Offsets of 1 are subtracted to prevent the bounding box border lines from interfering with results
			}
		}
		else
		{
			LOG("Motion Switched Off - After 4th point is selected");	
			isfourthPointSelected = isSecondPointSelected = false;
			isThirdPointSelected = false;  inMotion = false;
		}
	}
}

void Translate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Translate");
	
	if(!isSecondPointSelected)
	{
		LOG("Second Point Selected");
		secondPoint.set(X_AXIS, mouseX);
		secondPoint.set(Y_AXIS, mouseY);

		isSecondPointSelected = true;
		copyFromTo(img, imageDataBefore);			
		inMotion = true;
	}
	else if(!isThirdPointSelected)
	{
		LOG("Third Point Selected");
		if(inMotion)
		{
			glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
			glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		
			glBegin(GL_LINE_LOOP);
				glVertex2f(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
				glVertex2f(secondPoint.get(X_AXIS), mouseY);
				glVertex2f(mouseX, mouseY);
				glVertex2f(mouseX, secondPoint.get(Y_AXIS));			
			glEnd();
			thirdPoint.set(X_AXIS, mouseX);
			thirdPoint.set(Y_AXIS, mouseY);
		}
		else
		{
			LOG("Motion Switched Off");	
			isThirdPointSelected = true;
			inMotion = true;

			fourthPointSelection(mouseX, mouseY);
		}
	}
	else
		fourthPointSelection(mouseX, mouseY);
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
	glPixelZoom(2,2);
	glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
	//glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
}

//**********************************************************************************************************************//

class Rotate : public Tool
{
private:
	Coordinates secondPoint;
	bool isSecondPointSelected;
	Coordinates thirdPoint;
	bool isThirdPointSelected;
	Coordinates fourthPoint;
	bool isfourthPointSelected;
	float rotationAngle;
public:
	Rotate(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
};

Rotate::Rotate(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	isSecondPointSelected = false;
	rotationAngle = 0;
}
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
	static bool inMotion = false;
	if(!isSecondPointSelected)
	{
		cout<<"Second Point Selected"<<endl;
		secondPoint.set(X_AXIS, mouseX);
		secondPoint.set(Y_AXIS, mouseY);

		isSecondPointSelected = true;
		copyFromTo(img, imageDataBefore);			
	}
	else if(!isThirdPointSelected)
	{
		cout<<"Third Point Selected"<<endl;
		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);


		glBegin(GL_LINE_LOOP);
			glVertex2f(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
			glVertex2f(secondPoint.get(X_AXIS), mouseY);
			glVertex2f(mouseX, mouseY);
			glVertex2f(mouseX, secondPoint.get(Y_AXIS));			
		glEnd();
		thirdPoint.set(X_AXIS, mouseX);
		thirdPoint.set(Y_AXIS, mouseY);
		isThirdPointSelected = true;
		inMotion = false;
		isfourthPointSelected = false;
		isThirdPointSelected = true;  
	}
	rotationAngle += 5;
	if(rotationAngle>360) rotationAngle -= 360;
	glRotatef(rotationAngle,0,0,1);
	glBegin(GL_LINE_LOOP);
			glVertex2f(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
			glVertex2f(secondPoint.get(X_AXIS), mouseY);
			glVertex2f(mouseX, mouseY);
			glVertex2f(mouseX, secondPoint.get(Y_AXIS));			
	glEnd();
	glRotatef(-rotationAngle,0,0,1);
	/*for(int i = firstPoint.get(X_AXIS);i<secondPoint.get(X_AXIS);i++)
	{
		for(int j=firstPoint.get(Y_AXIS);j>secondPoint.get(Y_AXIS);j--)
		{
			glRasterPos2i(i, j);
			glDrawPixels(1,1, GL_RGB,GL_FLOAT, img);
		}
	}*/
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

//**********************************************************************************************************************//

/*
 *Text (TeaPot) Tool - First select this tool from the toolbar. Then, select the text option from the menu bar. Type the required
 *		string using the TypeWriter. When done, press the Enter Key. Now, click and move the mouse around the canvas to see some
 *		magic.
 */

class Text : public Tool
{
private:
	string text;	

public:
	Text(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);

	void setText(string str);
};

Text::Text(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	text = "Text Not Entered";
}
void Text::render()
{
	LOG("Render Text");
	glColor3f(1, 1, 1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(0, 0, 0);
	drawText("Text", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
	glColor3f(0, 0, 0);
	//glutWireTeapot(4);
}

void Text::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw T-bag" << text);
	drawText(text.c_str(), mouseX, mouseY);	
}

void Text::setText(string str)
{	
	text = str;
	LOG("setText = " << text);
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