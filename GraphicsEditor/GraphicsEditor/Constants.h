#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#define DEBUG 0
#if DEBUG
#define LOG(s) cout<<s<<endl;
#else
#define LOG(s) 
#endif


enum Size
{
	SMALL = 0,
	MEDIUM,
	LARGE
};
enum AxisType
{
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS
};
enum MenuOps
{
	NEW = 1,
	OPEN,
	SAVE,
	EXIT,
	CUT_PASTE,
	COPY_PASTE
};
enum ColorType
{
	BLACK,
	RED,
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	WHITE
};


#define APPLICATION_WINDOW_HEIGHT 800
#define APPLICATION_WINDOW_WIDTH 800
#define MULT_FACTOR 3
#define NAMSIZE 25

#define CANVAS_LEFT 10
#define CANVAS_RIGHT APPLICATION_WINDOW_WIDTH / 1.15
#define CANVAS_BOTTOM 10
#define CANVAS_TOP APPLICATION_WINDOW_HEIGHT / 1.15

#define COLOR_PANEL_LEFT CANVAS_RIGHT + 10
#define COLOR_PANEL_RIGHT COLOR_PANEL_LEFT + 90
#define COLOR_PANEL_TOP CANVAS_TOP
#define COLOR_PANEL_BOTTOM CANVAS_BOTTOM

#define TOOLBAR_LEFT CANVAS_LEFT
#define TOOLBAR_RIGHT COLOR_PANEL_RIGHT
#define TOOLBAR_BOTTOM CANVAS_TOP + 10
#define TOOLBAR_TOP TOOLBAR_BOTTOM + 70

#define MENUBAR_LEFT CANVAS_LEFT
#define MENUBAR_RIGHT COLOR_PANEL_RIGHT
#define MENUBAR_TOP APPLICATION_WINDOW_HEIGHT
#define MENUBAR_BOTTOM TOOLBAR_TOP
#define MENUBAR_NUM_ROWS 1
#define MENUBAR_NUM_COLS 7
#define MENUBAR_BORDER_WIDTH 70
#define MENUBAR_BORDER_HEIGHT (MENUBAR_TOP - MENUBAR_BOTTOM) / MENUBAR_NUM_ROWS

#define GAP 4
#define TOOLBAR_NUM_ROWS 2
#define TOOLBAR_NUM_COLS 8
#define TOOL_BORDER_WIDTH 90
#define TOOL_BORDER_HEIGHT 30

#define COLOR_SQUARE_SIDE 30
#define COLOR_PANEL_NUM_ROWS 5
#define COLOR_PANEL_NUM_COLS 3

#define POINT_INFO_X 10
#define POINT_INFO_Y 20

#define OP_NEW_X MENUBAR_LEFT + 10
#define OP_OPEN_X OP_NEW_X + 30
#define OP_SAVE_X OP_OPEN_X + 40
#define OP_EXIT_X OP_SAVE_X + 45
#define OP_CUPASTE_X OP_EXIT_X + 70
#define OP_COPY_X OP_CUPASTE_X + 70
#define OP_TYPEWRITER_X OP_COPY_X + 200

#define OP_Y MENUBAR_TOP - 20

#define SIZ_SELECTOR_LEFT COLOR_PANEL_LEFT
#define SIZ_SELECTOR_BOTTOM 420
#define SIZ_SELECTOR_TOP (SIZ_SELECTOR_BOTTOM+COLOR_SQUARE_SIDE)
#define SIZ_SELECTOR_RIGHT COLOR_PANEL_RIGHT


#endif