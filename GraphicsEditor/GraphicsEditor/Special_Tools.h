#include <cmath>
#include <vector>
#include "Tool.h"
#include "Color.h"
#include "Callback.h"
//The tool.h header file was very big to this one!
class Translate : public Tool
{
private:
	Coordinates pastePoint;
	bool ispastePointSelected;

protected:
	Color selectedColor;
	Coordinates selectionBoxFirstPoint;
	bool isselectionBoxFirstPointSelected;
	Coordinates selectionBoxSecondPoint;
	bool isselectionBoxSecondPointSelected;	

	bool inMotion;

	bool eraseSelection;
	
	void drawDashedLineLoop(float x1, float y1, float x2, float y2);
	virtual void selectionBoxLogic(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY, Coordinates &point, SingularCallBack<Translate, void, int, int>* callback);

public:
	Translate(float x1, float y1, float x2, float y2);
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
	void select();
	void stop();
	void convertToCutOperator();

	virtual void mouseDragPointSelection(int mouseX, int mouseY);	//After the selection box has been defined by the user...
};

Translate::Translate(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	ispastePointSelected = isselectionBoxFirstPointSelected = isselectionBoxSecondPointSelected = false;
	inMotion = false;

	eraseSelection = false;
}

void Translate::stop()
{	
	inMotion = false;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	LOG("Motion Switched Off - STOP Function");			
}

void Translate::convertToCutOperator()
{
	eraseSelection = true;
}

void Translate::drawDashedLineLoop(float x1, float y1, float x2, float y2)
{
	glPushAttrib(GL_ENABLE_BIT); 
	glPushAttrib(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 1, 1);
		glLineStipple(1, 0x0C0F); 
		glEnable(GL_LINE_STIPPLE);

		glBegin(GL_LINE_LOOP);
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y2);
			glVertex2f(x2, y1);			
		glEnd();
	glPopAttrib();
	glPopAttrib();	
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

void Translate::selectionBoxLogic(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY, Coordinates &point, SingularCallBack<Translate, void, int, int>* callback)
{
	if(!isselectionBoxFirstPointSelected)
	{
		LOG("Second Point Selected");
		selectionBoxFirstPoint.set(X_AXIS, mouseX);
		selectionBoxFirstPoint.set(Y_AXIS, mouseY);

		isselectionBoxFirstPointSelected = true;
		copyFromTo(img, imageDataBefore);			
		inMotion = true;

		selectionBoxSecondPoint = selectionBoxFirstPoint;

		//Tracking the color
		Tool::getCurrentColor(selectedColor);	

		return;
	}
	
	
	if(!isselectionBoxSecondPointSelected)
	{
		LOG("Third Point Selected");
		if(inMotion)
		{
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);

			//Removing old rectangle by xoring again				
			selectedColor.setInvertedGLColor();	
			drawDashedLineLoop(selectionBoxFirstPoint.get(X_AXIS), selectionBoxFirstPoint.get(Y_AXIS), selectionBoxSecondPoint.get(X_AXIS), selectionBoxSecondPoint.get(Y_AXIS));				
			glFlush();

			//Drawing new rectangle
			drawDashedLineLoop(selectionBoxFirstPoint.get(X_AXIS), selectionBoxFirstPoint.get(Y_AXIS), mouseX, mouseY);			
			glFlush();

			glDisable(GL_COLOR_LOGIC_OP);
			glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
			selectionBoxSecondPoint.set(X_AXIS, mouseX);
			selectionBoxSecondPoint.set(Y_AXIS, mouseY);		
		}
		else
		{
			LOG("Motion Switched Off");	
			isselectionBoxSecondPointSelected = true;
			inMotion = true;
			
			//Recording Original End point of bounding box
			selectionBoxFirstPoint.setToBoundingBoxCoordinates(selectionBoxSecondPoint);	
			point = selectionBoxSecondPoint;				
			callback->execute(mouseX, mouseY);			
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		}

		return;
	}
	callback->execute(mouseX, mouseY);	
}

void Translate::mouseDragPointSelection(int mouseX, int mouseY)
{
	if(!ispastePointSelected)
	{
		LOG("Before motion check -- Fourth Point Selected");	
		if(inMotion)
		{
			LOG("Fourth Point Selected");		
		
			selectionBoxFirstPoint.setToBoundingBoxCoordinates(selectionBoxSecondPoint);	

			glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
			glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		
			LOG("Bounding Box width = " << (selectionBoxSecondPoint.get(X_AXIS) - selectionBoxFirstPoint.get(X_AXIS)) / 2.0);
			LOG("Mouse X = " << mouseX << "\tMouseY = " << mouseY);

			pastePoint.set(X_AXIS, mouseX - (selectionBoxSecondPoint.get(X_AXIS) - selectionBoxFirstPoint.get(X_AXIS)) / 2.0);
			pastePoint.set(Y_AXIS, mouseY - (selectionBoxSecondPoint.get(Y_AXIS) - selectionBoxFirstPoint.get(Y_AXIS)) / 2.0);

			LOG("Fourth Point After = " << pastePoint);

			//Copies the contents of the bounding box of the selection to another part of the canvas, the bounding box of which has its bottom left coordinates at fourthPoint
			glRasterPos2f(pastePoint.get(X_AXIS), pastePoint.get(Y_AXIS));
			glCopyPixels(selectionBoxFirstPoint.get(X_AXIS), selectionBoxFirstPoint.get(Y_AXIS), selectionBoxSecondPoint.get(X_AXIS) - selectionBoxFirstPoint.get(X_AXIS) - 1, selectionBoxSecondPoint.get(Y_AXIS) - selectionBoxFirstPoint.get(Y_AXIS) - 1, GL_COLOR);
		

			if(eraseSelection)
			{
				//Cut operation
				glColor3f(1, 1, 1);
				glRectf(selectionBoxFirstPoint.get(X_AXIS) - 1, selectionBoxFirstPoint.get(Y_AXIS) - 1, selectionBoxSecondPoint.get(X_AXIS), selectionBoxSecondPoint.get(Y_AXIS));	//Offsets of 1 are subtracted to prevent the bounding box border lines from interfering with results
			}
		}
		else
		{
			LOG("Motion Switched Off - After 4th point is selected");	
			ispastePointSelected = isselectionBoxFirstPointSelected = false;
			isselectionBoxSecondPointSelected = false;  inMotion = false;
		}
	}
}

void Translate::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Translate");
	this->canvas = canvas;

	selectionBoxLogic(canvas, img, mouseX, mouseY, pastePoint, new SingularCallBack<Translate, void, int, int>(this, &Translate::mouseDragPointSelection));	
}
void Translate::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
//**********************************************************************************************************************//


class Scale : public Translate		//Inherited from Translate because of the similar selection logic
{
private:
	Coordinates zoomPoint;
	bool iszoomPointSelected;

	GLfloat buffer[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR];

protected:
	virtual void mouseDragPointSelection(int mouseX, int mouseY);

public:
	Scale(float x1, float y1, float x2, float y2);
	void select();
	void render();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};

Scale::Scale(float x1, float y1, float x2, float y2):Translate(x1,y1,x2,y2)
{
	iszoomPointSelected = false;
}

void Scale::render()
{
	LOG("Render Scale");
	glColor3f(0, 0, 0);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Scale", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 5, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);
}

void Scale::mouseDragPointSelection(int mouseX, int mouseY)
{
	LOG("ZOOM POINT SELECTION");

	if(!iszoomPointSelected)
	{
		LOG("Before motion check -- Zoom Point Selected");	
		if(inMotion)
		{
			LOG("Zoom Point Selected IN MOTION");

			//Changing the zoom box dimensions, and hence, the scale factor, as the mouse moves
			selectionBoxSecondPoint.set(X_AXIS, mouseX);
			selectionBoxSecondPoint.set(Y_AXIS, mouseY);

			selectionBoxFirstPoint.setToBoundingBoxCoordinates(selectionBoxSecondPoint);	
		
			//Erase selection box and return to state before any zoom operation was performed
			glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
			glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);

			//Read the pixels inside the selection box
			glReadPixels(selectionBoxFirstPoint.get(X_AXIS), selectionBoxFirstPoint.get(Y_AXIS), selectionBoxSecondPoint.get(X_AXIS)- selectionBoxFirstPoint.get(X_AXIS), selectionBoxSecondPoint.get(Y_AXIS)- selectionBoxFirstPoint.get(Y_AXIS), GL_RGB, GL_FLOAT, buffer);		

			double scaleFactorX = (selectionBoxSecondPoint.get(X_AXIS)- selectionBoxFirstPoint.get(X_AXIS)) / (zoomPoint.get(X_AXIS) - selectionBoxFirstPoint.get(X_AXIS));
			double scaleFactorY	= (selectionBoxSecondPoint.get(Y_AXIS)- selectionBoxFirstPoint.get(Y_AXIS)) / (zoomPoint.get(Y_AXIS) - selectionBoxFirstPoint.get(Y_AXIS));

			LOG("SCALE FACTORS = " << scaleFactorX << "\t" << scaleFactorY);		

			//Draw a scaled version of the selection with origin at the first point of the selection box
			glPixelZoom(scaleFactorX, scaleFactorY);
			glRasterPos2i(selectionBoxFirstPoint.get(X_AXIS), selectionBoxFirstPoint.get(Y_AXIS));			
			glDrawPixels(selectionBoxSecondPoint.get(X_AXIS)- selectionBoxFirstPoint.get(X_AXIS), selectionBoxSecondPoint.get(Y_AXIS)- selectionBoxFirstPoint.get(Y_AXIS), GL_RGB,GL_FLOAT, buffer);
			glPixelZoom(1, 1);		//Return to normal			
		}
		else
		{
			LOG("Motion Switched Off - After zoom point is selected");	
			iszoomPointSelected = isselectionBoxFirstPointSelected = false;
			isselectionBoxSecondPointSelected = false;  inMotion = false;
		}
	}	
}

void Scale::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void Scale::drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY)
{
	LOG("Draw Scale");
	this->canvas = canvas;

	selectionBoxLogic(canvas, img, mouseX, mouseY, zoomPoint, new SingularCallBack<Translate, void, int, int>(this, &Translate::mouseDragPointSelection));	
}

//**********************************************************************************************************************/

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
private:
	class Node
	{	
	
		int x;
		int y;
		Color color;

	public:
		Node(int x, int y)
		{
			this->x = x;
			this->y = y;			
		}

		Color getColor(GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3])
		{
			if(x < 0 || x >= CANVAS_RIGHT - CANVAS_LEFT)
				return color;
			if(y < 0 || y >= CANVAS_TOP - CANVAS_BOTTOM - 1)
				return color;
			/*LOG("RIGHT - LEFT = " << CANVAS_RIGHT - CANVAS_LEFT);
			LOG("TOP - BOTTOM = " << CANVAS_TOP - CANVAS_BOTTOM);
			LOG("x = " << x << "\ty = " << y << "\t" << img[y][x][0] << "\t" << img[y][x][1] << "\t" << img[y][x][2]);*/
			Color color(img[y][x][0], img[y][x][1], img[y][x][2]);
			this->color = color;
			return color;
		}

		void setColor(GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3], Color replacementColor)
		{
			img[y][x][0] = replacementColor.getRed();
			img[y][x][1] = replacementColor.getGreen();
			img[y][x][2] = replacementColor.getBlue();			
		}

		int getX(){return x;}
		int getY(){return y;}
	};

	Color *fillColor;
	GLfloat buffer[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3];
	bool fillingInProgress;

	void fill(int x, int y, Color *targetColor, Color *replacementColor, GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3]);
	void queueFill(int x, int y, Color *targetColor, Color *replacementColor, GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3]);

public:
	FloodFiller(float x1, float y1, float x2, float y2);
	void render();
	void select();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);	
	void Fill(int, int, Color &, int depth);
};

FloodFiller::FloodFiller(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	fillingInProgress = false;
}

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
	
	if(!fillingInProgress)
	{
		fillingInProgress = true;
		glReadPixels(CANVAS_LEFT, CANVAS_BOTTOM, CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB, GL_FLOAT, buffer);	
	
		GLfloat rgbValues[3];	
		glReadPixels(mouseX - CANVAS_LEFT, mouseY - CANVAS_BOTTOM, 1, 1, GL_RGB, GL_FLOAT, rgbValues);
		Color *targetColor = new Color(rgbValues[0],rgbValues[1],rgbValues[2]);	
		Color *replacementColor = new Color(1, 0, 0);
		Tool::getCurrentColor(*replacementColor);	

		LOG("Target Colour = " << *targetColor);	
		LOG("Replacement Colour = " << *replacementColor);	
		glPointSize(1);	

		//fill(mouseX, mouseY, targetColor, replacementColor, buffer);
		queueFill(mouseX - CANVAS_LEFT, mouseY - CANVAS_BOTTOM, targetColor, replacementColor, buffer);

		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, buffer);

		fillingInProgress = false;
	}
}	

void FloodFiller::queueFill(int x, int y, Color *targetColor, Color *replacementColor, GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3])
{
	//LOG("QUEUE FILL");

	vector<Node> queue;
	Node startNode(x, y);
	queue.push_back(startNode);

	while(!queue.empty())
	{
		Node n = queue[queue.size() - 1];
		queue.pop_back();
		
		if(n.getColor(img) == *targetColor)
		{
			//LOG("I'm here...GET OUT FROM DEBUG MODE, OTHERWISE I WON'T END");
			n.setColor(img, *replacementColor);
			if(n.getX() - 1 >= 0)
				queue.push_back(Node(n.getX() - 1, n.getY()));
			if(n.getX() + 1 < CANVAS_RIGHT - CANVAS_LEFT)
				queue.push_back(Node(n.getX() + 1, n.getY()));
			if(n.getY() - 1 >= 0)
				queue.push_back(Node(n.getX(), n.getY() - 1));
			if(n.getY() + 1 < CANVAS_TOP - CANVAS_BOTTOM)
				queue.push_back(Node(n.getX(), n.getY() + 1));
		}
	}
}

void FloodFiller::fill(int x, int y, Color *targetColor, Color *replacementColor, GLfloat img[(int)(CANVAS_TOP - CANVAS_BOTTOM)][(int)(CANVAS_RIGHT - CANVAS_LEFT)][3])
{
	//Color node(img[y][3 * x], img[y][3 * x + 1], img[y][3 * x + 2]);
	Color *node = new Color(img[y][x][0], img[y][x][1], img[y][x][2]);

	if(!(*node == *targetColor))
	{
		LOG("TERMINATING FILL");
		return;
	}

	LOG("CHECKING");

	LOG("Color of node before = " << img[y][x][0] << "\t" << img[y][x][1] << "\t" << img[y][x][2]);

	img[y][x][0] = replacementColor->getRed();
	img[y][x][1] = replacementColor->getGreen();
	img[y][x][2] = replacementColor->getBlue();

	LOG("Color of node after = " << img[y][x][0] << "\t" << img[y][x][1] << "\t" << img[y][x][2]);
	if(x - 1 >= 0)
		fill(x - 1, y, targetColor, replacementColor, img);
	if(x + 1 < CANVAS_RIGHT - CANVAS_LEFT)
		fill(x + 1, y, targetColor, replacementColor, img);
	if(y - 1 >= 0)
		fill(x, y - 1, targetColor, replacementColor, img);
	if(y + 1 < CANVAS_TOP - CANVAS_BOTTOM)
		fill(x, y + 1, targetColor, replacementColor, img);	
}



void FloodFiller::Fill(int x, int y, Color &previousPixelColor, int depth)
{	
	if(depth <=0 ) 
		return;

	GLfloat rgbValues[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, rgbValues);
	/*Color currentColor(rgbValues[0],rgbValues[1],rgbValues[2]);

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
	return;*/
}
void FloodFiller::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_HELP);
}


//**********************************************************************************************************************//



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
	void select();
	void setText(string str);
};

Text::Text(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{
	text = "Text Not Entered";
}
void Text::render()
{
	LOG("Render Text");
	glColor3f(0.8, 0.4, 0.2);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
	drawText("Text", (top_right->get(X_AXIS) + bottom_left->get(X_AXIS)) / 2.0 - BITMAP_CHARACTER_WIDTH * 4, (top_right->get(Y_AXIS) + bottom_left->get(Y_AXIS)) / 2.0 - BITMAP_CHARACTER_HEIGHT);	
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
void Text::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_TEXT);
}
//**********************************************************************************************************************//


class InsideClipper : public Tool
{
public:
	InsideClipper(float x1, float y1, float x2, float y2);
	void render();
	void select();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};
InsideClipper:: InsideClipper(float x1, float y1, float x2, float y2):Tool(x1,y1,x2,y2)
{}
void InsideClipper::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
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
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{		
		Coordinates secondPoint(mouseX, mouseY, 0);
		firstPoint.setToBoundingBoxCoordinates(secondPoint);

		glRasterPos2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glDrawPixels(CANVAS_RIGHT - CANVAS_LEFT, CANVAS_TOP - CANVAS_BOTTOM, GL_RGB,GL_FLOAT, imageDataBefore);
		glColor3f(1,1,1);
		glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);		
		//Clip the right part
		glBegin(GL_POLYGON);
		glVertex2i(secondPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
		glVertex2i(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
		glVertex2i(CANVAS_RIGHT, CANVAS_TOP);
		glVertex2i(CANVAS_RIGHT, CANVAS_BOTTOM);
		glEnd();
		
		//Clip the left part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glVertex2i(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
		glVertex2i(firstPoint.get(X_AXIS), secondPoint.get(Y_AXIS));		
		glVertex2i(CANVAS_LEFT, CANVAS_TOP);
		glEnd();

		//clip the top part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_TOP);
		glVertex2i(firstPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
		glVertex2i(secondPoint.get(X_AXIS), secondPoint.get(Y_AXIS));
		glVertex2i(CANVAS_RIGHT, CANVAS_TOP);
		glEnd();
		
		//clip the bottom part
		glBegin(GL_POLYGON);
		glVertex2i(CANVAS_LEFT, CANVAS_BOTTOM);
		glVertex2i(firstPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
		glVertex2i(secondPoint.get(X_AXIS), firstPoint.get(Y_AXIS));
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
	void select();
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
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		isFirstPointSelected = false;
		copyFromTo(img, imageDataBefore);				
	}
	else
	{	
		glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
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
void OutClipper::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
class RingDrawer : public Tool
{
public:
	RingDrawer(float x1, float y1, float x2, float y2);
	void render();
	void select();
	void drawOnCanvas(Canvas *canvas, GLfloat img[APPLICATION_WINDOW_HEIGHT][APPLICATION_WINDOW_WIDTH * MULT_FACTOR], int mouseX, int mouseY);
};

//**********************************************************************************************************************//
void RingDrawer::select()
{
	glColor4f(0, 0, 0, 0.5);			//Using a tranlucent polygon
	glRectf(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS), top_right->get(X_AXIS), top_right->get(Y_AXIS));

	glColor4f(1, 1, 1, 0.5);			//Using a tranlucent line
	glBegin(GL_LINES);
		glVertex2f(bottom_left->get(X_AXIS), bottom_left->get(Y_AXIS) );
		glVertex2f(top_right->get(X_AXIS), top_right->get(Y_AXIS));
	glEnd();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
RingDrawer::RingDrawer(float x1, float y1, float x2, float y2):Tool(x1, y1, x2, y2)
{}

void RingDrawer::render()
{	
	glColor3f(0.2, 0.5, 0.1);
	glRectf(bottom_left->get(X_AXIS) + 2, bottom_left->get(Y_AXIS) + 1, top_right->get(X_AXIS) - 2, top_right->get(Y_AXIS) - 2);
	glColor3f(1, 1, 1);
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