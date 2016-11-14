/* file: mouse.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Mouse Driver Interface")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *	3D Cable analysis
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      This Software was Designed using the following development tools:
 *
 *      MS-DOS Verison 3.3
 *	Microsoft C Version 6.00a
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

  #include	<dos.h>

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

 MOUSE mouse;

 int mouse_flag;

 #define NO_MOUSE  0
 #define YES_MOUSE 1

/****************************************************************************
 *
 *	Mouse Definitions
 *
 ***************************************************************************/

 #define MOUSE_INT	0x33	/* Microsoft Mouse Standard Interrupt number */
 #define CURSOR_OFF	1	/* INT 33 function number */
 #define CURSOR_ON	2	/* INT 33 function number */
 #define GET_LOCATION	3	/* INT 33 function number */
 #define SET_LOCATION	4	/* INT 33 function number */

 #define TEXT_OFFSET	1	/* offset of test columns, rows to graphics */

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

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

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int mousecheck(void)		       /* see if mouse exists, & # of buttons */
 {
	union REGS inregs;
	union REGS outregs;
	int mouse_check;

	inregs.x.ax = 0;
	int86(MOUSE_INT,&inregs,&outregs);

	mouse_check = (outregs.x.ax ? outregs.x.bx : 0);

	if(mouse_check == 0) {
		mouse_flag = NO_MOUSE;
		return(0);
		}

	mouse_flag = YES_MOUSE;

	return(mouse_check);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int mousebuttons(void)		      /* see if mouse buttons are pressed */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return(0);

	inregs.x.ax = 6;
	int86(MOUSE_INT,&inregs,&outregs);

	return(outregs.x.ax & 3);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int mouseclick(void)		       /* see if mouse buttons were clicked */
 {
	int click = 0;
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return(0);

	inregs.x.ax = 5;
	inregs.x.bx = 1;
	int86(MOUSE_INT,&inregs,&outregs);
	click = outregs.x.bx << 1;
	inregs.x.bx--;
	int86(MOUSE_INT,&inregs,&outregs);

	return(click | outregs.x.bx);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void wait_mouseclick(void)		       /* see if mouse buttons were clicked */
 {
	int click = 0;
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	while(mouseclick()==0);	/* wait for mouse buttons to be clicked */

	return;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mousecursor(int toggle)	       /* turn mouse cursor on or off */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	if (toggle)
	    inregs.x.ax = CURSOR_OFF;
	else
	    inregs.x.ax = CURSOR_ON;

	int86(MOUSE_INT,&inregs,&outregs);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mouseloc(int column, int row)	/* set location of mouse cursor */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	inregs.x.ax = SET_LOCATION;
	inregs.x.cx = column;
	inregs.x.dx = row;
	int86(MOUSE_INT,&inregs,&outregs);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int mousecol(void)		       /* get column where mouse cursor is */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return(0);

	inregs.x.ax = GET_LOCATION;
	int86(MOUSE_INT,&inregs,&outregs);

	return(outregs.x.cx);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int mouserow(void)		       /* get row where mouse cursor is */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return(0);

	inregs.x.ax = GET_LOCATION;
	int86(MOUSE_INT,&inregs,&outregs);

	return(outregs.x.dx);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mousepos(void)
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	inregs.x.ax = GET_LOCATION;
	int86(MOUSE_INT,&inregs,&outregs);

	mouse.column_graphic = (unsigned)(outregs.x.cx);
	mouse.row_graphic = (unsigned)(outregs.x.dx);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mouseloc_text(int column, int row)	/* set location of mouse cursor */
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	inregs.x.ax = SET_LOCATION;
	inregs.x.cx = (column - TEXT_OFFSET) * 8;
	inregs.x.dx = (row - TEXT_OFFSET) * 8;

	int86(MOUSE_INT,&inregs,&outregs);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mouse_window(int top, int bot, int left, int right)
 {
	union REGS inregs;
	union REGS outregs;

	if(mouse_flag == NO_MOUSE)
		return;

	inregs.x.ax = 7;
	inregs.x.cx = (left - TEXT_OFFSET) * 8;
	inregs.x.dx = (right - TEXT_OFFSET) * 8;

	int86(MOUSE_INT,&inregs,&outregs);

	inregs.x.ax = 8;
	inregs.x.cx = (top - TEXT_OFFSET) * 8;
	inregs.x.dx = (bot - TEXT_OFFSET) * 8;

	int86(MOUSE_INT,&inregs,&outregs);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mouse_box( int row, int col, int rowLast, int colLast )
 {
 int set_row,set_col;

	if(mouse_flag == NO_MOUSE)
		return;

	mouse.top = row;
	mouse.bottom = row + rowLast - 1;
	mouse.left = col + 1;
	mouse.right = col + colLast - 1;

	mouse.row = mouse.row_last = mouse.left;
	mouse.column = mouse.column_last = mouse.top;

	mouse.up_down_flag = 0;
	mouse.left_right_flag = 0;

	set_row = (mouse.top - TEXT_OFFSET) * 8;
	set_col = (mouse.left - TEXT_OFFSET) * 8;

	mouseloc(set_col, set_row);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void mouse_house(void)
 {
 int set_row,set_col;
 int set;

	if(mouse_flag == NO_MOUSE) {
		mouse.row_last = mouse.row = 0;
		mouse.column_last = mouse.column = 0;
		return;
		}

	set = 0;
	mouse.row_last = mouse.row;
	mouse.column_last = mouse.column;

	mousepos();
	mouse.column = (mouse.column_graphic/8) + TEXT_OFFSET;
	mouse.row = (mouse.row_graphic/8) + TEXT_OFFSET;

	set_col = mouse.column;
	set_row = mouse.row;

	if(mouse.row > mouse.bottom) {
		set_row = mouse.bottom;
		set = 1;
		}
	if(mouse.row < mouse.top) {
		set_row = mouse.top;
		set = 1;
		}
	if(mouse.column > mouse.right) {
		set_col = mouse.right;
		set = 1;
		}
	if(mouse.column < mouse.left) {
		set_col = mouse.left;
		set = 1;
		}
	if(set!=0) {
		mouse.row = set_row;
		mouse.column = set_col;
		set_row = (set_row - TEXT_OFFSET) * 8;
		set_col = (set_col - TEXT_OFFSET) * 8;
		mouseloc(set_col, set_row);
		}
 }
