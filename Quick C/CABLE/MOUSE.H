/****************************************************************************
 *
 *	Mouse Structure
 *
 ***************************************************************************/

 typedef struct _MOUSE {
	unsigned int row_graphic;
	unsigned int column_graphic;
	int top;
	int bottom;
	int left;
	int right;
	int row;
	int column;
	int row_last;
	int column_last;
	int up_down_flag;
	int left_right_flag;
	} MOUSE;

 extern MOUSE mouse;

 extern int mouse_flag;

 #define NO_MOUSE  0
 #define YES_MOUSE 1

 int mousecheck(void);			/* see if mouse exists, & # of buttons */
 int mousebuttons(void);		/* see if mouse buttons are pressed */
 int mouseclick(void);			/* see if mouse buttons were clicked */
 void wait_mouseclick(void);		/* wait for mouse buttons click */
 void mousecursor(int toggle);		/* turn mouse cursor on or off */
 void mouseloc(int column, int row);	/* set location of mouse cursor */
 int mousecol(void);			/* get column where mouse cursor is */
 int mouserow(void);			/* get row where mouse cursor is */
 void mousepos(void);			/* get row and column where mouse cursor is */
 void mouseloc_text(int column, int row);
 void mouse_window(int top, int bot, int left, int right);
					/* set window of mouse movement */
 void mouse_box( int row, int col, int rowLast, int colLast );
 void mouse_house(void);
