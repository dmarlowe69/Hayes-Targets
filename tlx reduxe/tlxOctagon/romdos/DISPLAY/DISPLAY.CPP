/*

	This software, including source code, is provided as is. It has been
        tested under a variety of conditions but Octagon Systems Corporation (TM)
        makes no warranty of any kind, express or implied for its specific
        use with your application. Under no circumstances whatsoever
        shall Octagon Systems Corporation (TM) be held liable for any damages,
        including any lost profits, lost savings, or other incidental or
        consequential damages arising from the use of, or inablity to use,
        this software.

	Filename:		DISPLAY.cpp

	Function name:		display

	Received parameters:	none

	Returned parameters:	none

	Edit date:		3-19-91

	Function:		Installs tsr to scan the keypad or handle
				2x20 vf, 4x20 vf or 4x40 lcd displays either
				with the DP-IFB or VF-IFB card connected
				either to line printer port, or to an
				829/5600 card. The 4010 can be connected via
				2010 interface card.

	Revisions:
  10-03-91   EH   Change to access using Ln so multiple printers can be used
  02-21-92   EH   Add LCD 4x20 support, fix axxx configuration of 8255 v3.0.
  02-27-92   EH   Add delay after vf clear for fast systems v3.1
  03-05-92   EH   Add LCD 2x40 displays. v3.2
  09-30-92   EH   Try not disabling ints in int17 routine. v3.3
  11-19-92   EH   Try more delays for faster systems on vf. v3.4
  11-24-92   HK   Fixed bug calling other LPTs, upgraded to Borland C++.
  11-25-92   EH   Add 2x20 LCD (D6) v3.5
  12-28-92   EH   Add strip down options (no keypad/no menu) v3.6
  06-03-93   EH   Add menu for /w options, add '\' support v3.7
  11-09-93   EH   Add timing fixes in VF_OUT v3.8
  11-16-93   EH   Separate out display type 1 and 2 in VF_OUT v3.9
  08-19-94   EH   Add K3 - (8x8 touchpad on PC450 LPT) v4.0
  10-10-94   EH   Remove K3 v4.1
		  R - allow installation for 2nd display/keypad
    *** This requires all requests (even keypads) to send lpt number ***
    *** subfunction 3 has changed, code written for previous display ***
    *** will not work now ****
		  S - no scrolling option
		  remove W1, W2 options and do automatic timing.
		  automatic sizing of tsr and reduce stack and heap
		  allow debounce count = 0 (no debounce)
		  add uninstall option 10 (removes interrupts/leaves memory)
		  add enable/disable keypad scanning option (11).
		  Improve keypad scanning algorithm to speed system up.
  10-07-94   EH   Remove K3 v4.1
  12-12-95   EH	  Pass all registers (except flags) if not our int 17 v4.2
  06-02-96   HK	  Support EZIO. Configure differently if EZIO is detected
		  Bump to ver. 4.3

  Notes:
    When using a digital I/O port (instead of a line printer) the following
    port usage applies:
      Display: Output on port A.
      4x4 keypad: Output on upper port c, Input on lower port c.
      4x8 keypad: Output on port b, input on port c.
    When using a line printer the following applies (lpt=line printer base
    address):
      Display: Output on Write Data (lpt+0)
      4x4 keypad: Output on Write Control (lpt+2) lower 4 bits,
		  Input on Read status (lpt+1) upper 4 bits
		  Note: some of these lines are inverted logic.
      4x8 keypad: not available.

*/

#pragma inline			// for the imbedded assembly

#define enable_keypad 1         // change this to 0 to disable keypad
#define helpmenu 1		// change this to 0 to disable help menu
#define longmsgs 1		// change this to 0 to disable long messages

#define EZDIR_CMD 2
#define EZ8255_CMD 3

/* Includes */

#include <stdio.h>
#include <dos.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <bios.h>

/* reduce heaplength and stacklength to make a smaller program in memory */
extern unsigned _heaplen = 1024;
extern unsigned _stklen  = 512;

/* Common variables (both keypad and display) */
int num_lpts,			/* number of lpts defined	*/
    lpt_ports [3],		/* array of lpt port definitions */
    cur_lptn,			/* lpt number for accessing display/keypad */
    using_lpt,			/* using line printer device vs 8255 */
    dev_add = 0,		/* device base address	 	*/
    display_type = 0,		/* display type	0,1,2,3		*/
    keypad_type = 0,		/* keypad type 0,1,2,3		*/
    Scrolling = 1,		/* automatic scroll for displays*/
    keypadScanning = 1;         /* enable scanning of keypad    */

/* Display variables for LCD displays */
unsigned last_prn_char=0;	/* last character sent to printer*/
unsigned char enable_lg=0x80;	/* line group enable 		 */

#define max_lcd_rows 4
#define max_lcd_cols 40
int max_row = max_lcd_rows-1,
    max_col = max_lcd_cols-1;

unsigned char lcd_buf [max_lcd_rows][max_lcd_cols],
	      blinking = 0,	/* blinking cursor flag		 */
	      cur_on = 1;	/* cursor on flag		 */

int cur_row = 1,		/* current lcd line #		 */
    cur_col = 0;

#if enable_keypad
#define maxkeypad '2'
/* Keypad scan variables */
int last_row = 0,		/* last row hit for debounce chk */
    last_col = 0,		/* last col hit			 */
    kprls = 1;			/* keypad released flag		 */
unsigned char pkpc = 0;		/* keypad character being pressed*/

int key_debounce = 1,		/* debounce min number of times	 */
    key_deb_count = 0;		/* debounce counter		 */

int key_repeat = 0;		/* repeat flag = 0 no repeat	 */

#define key_buf_len 8		/* keypad buffer length		 */
int key_buf_first = 0,		/* keypad buffer pointers	 */
    key_buf_last = 0,
    key_count = 0;		/* keypad count of keys in buffer*/
unsigned char key_buf [key_buf_len]; /* keypad buffer	 	 */

/* interrupt variables */
#define	itr_1c 0x1c		/* timer interrupt (18.2 times second) */
void interrupt (*old_1c_vec)(...); /* storage for old int 1c interrupt */

#endif  // end of keypad definitions

#define itr_17 0x17
void interrupt (*old_17_vec)(...); /* storage for old int 17 interrupt */

/* Program */
unsigned long countTick;

unsigned long countTick1;

int ezioFlag = 0;

void delay1 (void)
{
  unsigned long delay1Count;
  for (delay1Count = 0; delay1Count < countTick1; delay1Count++);
}

void determineSpeed (void)
#define	biosTick    ((unsigned long far *)  0x0040006c)
{
  unsigned long lastTick;
  unsigned long count = 0;
  int tickSamples = 0;

  countTick = 1;

  lastTick = *biosTick;
  while (*biosTick == lastTick)
    if (count++ > 0x7FFFFFL) goto setTick;

  countTick = 0;
  while (tickSamples++ < 9) // check for about 1/2 second
  {
    lastTick = *biosTick;
    while (*biosTick == lastTick)
      countTick++;
  }

setTick:
  countTick1 = countTick >> 12;    // this is about 105 us...
  if (countTick1 == 0)
    countTick1 = 1;
  return;
}

/*
  LCD data output routine
  Input: d = data to output
*/

void lcdDelay(void)
{
  delay1();
  delay1();
  delay1();
}

void lcdout(unsigned char d)
{
unsigned int i;			/* delay loop counter */

  disable();
  outportb(dev_add, d | 0xc0);	/* output the data */
  lcdDelay();

  /* toggle the line to bring in the data */
  outportb(dev_add, d | enable_lg);
  lcdDelay();

  /* restore conditions	*/
  outportb(dev_add, d | 0xc0);
  lcdDelay();
  enable();
}

/*
   Print a character to the lcd display.
   The cursor is automatically advanced as set in initialization.
   Input: a = character to print
*/

void out_char_lcd(unsigned char a)
{
  lcdout(((a & 0xf0) >> 4) | 0x10);	/* send out high nibble */
  lcdout((a & 0x0f) | 0x10);		/* send out low nibble */
}

#if enable_keypad
void Read4x4LPT1(unsigned char *row, unsigned char *col)
{
static unsigned char KP4x4ctl1[] = {0xcb,0xc5,0xc6,0xc0,0xcc};

    // do quick check first for any key is pressed
    outportb(dev_add+2,KP4x4ctl1[0]);		// write column
    *row = ~(((inportb(dev_add+1) & 0xf0) >> 4) ^ 8) & 0xf;

    // if any key is pressed do long test to determine which one
    if (*row)
      for (*col = 0; *col < 4; (*col)++)
      {
	outportb(dev_add+2,KP4x4ctl1[(*col)+1]); // write column
	*row = ~(((inportb(dev_add+1) & 0xf0) >> 4) ^ 8) & 0xf;
	if (*row) break;
      }
}

void Read4x48255(unsigned char *row, unsigned char *col)
{
static unsigned char KP4x4ctl2[5] = {0xf0,0xef,0xdf,0xbf,0x7f};

    // do quick check first for any key is pressed
    outportb(dev_add+2,KP4x4ctl2[0]);		// write column
    *row = inportb(dev_add+2);			// read row

    // if any key is pressed do long test to determine which one
    if (*row)
      for (*col = 0; *col < 4; (*col)++)
      {
	outportb(dev_add+2,KP4x4ctl2[(*col)+1]); // write column
	*row = 0xf - (inportb(dev_add+2) & 0xf); // read row
	if (*row) break;
      }
}


void Read4x8(unsigned char *row, unsigned char *col)
{
static unsigned char KP4x8ctl[5] = {0x0f,0xfe,0xfd,0xfb,0xf7};

    // do quick check first for any key is pressed
    outportb(dev_add+1,KP4x8ctl[0]);		// write column
    *row = inportb(dev_add+2);			// read row

    // if any key is pressed do long test to determine which one
    if (*row)
      for (*col = 0; *col < 4; (*col)++)
      {
	outportb(dev_add+1,KP4x8ctl[(*col)+1]);	// write column
	*row = 0xff - (inportb(dev_add+2) & 0xff); // read row
	if (*row) break;
      }
}

/* key_decode */

/* Debounces the keypad row and column information and returns a
   key value from 1 to 16. Also allows repeats if enabled. */

int key_decode (unsigned int row, unsigned int column)
{
unsigned int kpc;

  /* Row and column must equal last values (debounce) to declare key was
     pressed. Otherwise just save old values and return. */
  if ((row != last_row) || (column != last_col))
  {
    last_col = column; /* save new key row/column */
    last_row = row;
    key_deb_count = 1; /* reset number of times the key has been pressed */
    kprls = 1;         /* indicate key was released */
    kpc = 0;
    pkpc = 0;
  }
  else
    if (key_deb_count != INT_MAX)
      key_deb_count ++;

  /* same key as last time but lets make sure it stays there */
  if ((key_debounce) && (key_deb_count <= key_debounce))
    kpc = 0; /* set key has not been pressed long enough */
  else
  {
    /* Change row (1,2,4,8) and column to kpc = 1-16 */
    kpc = 0;
    do {
      kpc ++;
      row = row >> 1;
    } while (row);

    if (keypad_type == 1)
      if (using_lpt)
	kpc = ((kpc-1) << 2) + column + 1;
      else
	kpc = (column << 2) + kpc;
    else
      kpc = (column << 3) + kpc;

    pkpc = kpc;

    /* if the key has not been repeated before then set the release */
    /* flag so we know whether its a repeat next time. */
    if (kprls)
    {
      kprls = 0;
      key_deb_count = 1;
    }
    else
      /* its a key repeat, if key repeats are not enabled return 0 */
      if (!key_repeat)
	kpc = 0;
      else
	key_deb_count = 1; /* set back so we repeat slowly */
  }
  return(kpc);
}

/* keypad_scan					      */

/* Keypad scan TSR which when installed in timer tick */
/* interrupt vector, checks for any keys pressed 18.2 */
/* times a second. Debounce and repeat options can be */
/* set with int_17 (also in this module). When a key  */
/* is pressed long enough, it is added to the keypad  */
/* buffer. Routines in int_17 indicate the status and */
/* return the keys pressed from the keypad buffer.    */

void interrupt keypad_scan(...)
{
static  unsigned char row = 0, column = 0; /* keypad row/col */
static  unsigned char kpc;  /* keypad character */

if (keypadScanning)
{
  switch (keypad_type)
  {
    case 1:
       if (using_lpt)
	 Read4x4LPT1 (&row, &column);
       else
	 Read4x48255 (&row, &column);
       break;
    case 2:
       Read4x8 (&row, &column);
       break;
    default:
       break;
  }

  /* get the key position if pressed	*/
  if (row)
  { // something was pressed
    kpc = key_decode(row,column);
    if (kpc)
    {
      /* then add the key to the buffer */
      disable();
      if (key_count != key_buf_len)
      {
	key_buf[key_buf_last++] = kpc;
	key_count++;
	/* check for wrap... move to beginning */
	if (key_buf_last == key_buf_len) key_buf_last = 0;
      }
      enable();
    }
  }
  else // clear out any key thats currently being pressed.
  if (pkpc) // if key was pressed before, but now its not clear out some
  {
      last_row = 0;
      kprls = 1;
      key_deb_count = 0;
      pkpc = 0;
  }
  /* call previously linked in vector */
} // keypadScanning
  old_1c_vec();
}
#endif // keypad routines

/************************************************************

  Display routines

 ************************************************************/
/*
  cursor_ctrl (cur_status)

  cur_status == 0  turn cursor off
  cur_status == 1  turn cursor on ( note blink flag )

*/

void cursor_ctrl(int cur_status)
{
  lcdout(0);
  if (cur_status == 0) /* if they want it off */
    lcdout(0xc);
  else
    if (blinking) /* they want if on and blinking */
      lcdout(0xd);
    else /* they want it on only */
      lcdout(0xe);
}

/*
  Set the DD (display data) ram position
  Inputs: lcdcol = column in LCD display
	  lcdrow = line number
*/

void setdd(int lcdcol,int lcdrow)
{
unsigned char line,d;
unsigned new_lg;

  cur_row = lcdrow;
  cur_col = lcdcol;

  if ((display_type == 4) && (cur_row > 2)) {
    lcdcol = lcdcol + max_col + 1;
  }

  if (display_type == 3) {
    /* figure which group to display to */
    if (lcdrow > 2)
      new_lg = 0x40;
    else
      new_lg = 0x80;

    /* check if were changing line groups. if so then disable cursor
       on old line group and enable it if needed on new line group */

    if (cur_on == 1)
      if (enable_lg != new_lg) {
	cursor_ctrl(0); /* turn cursor off on old line group */
	enable_lg = new_lg;
	cursor_ctrl(1); /* turn cursor on (if enabled) on new lg */
      }
    enable_lg = new_lg;
  }

  /* determine which line within that group is set (0 or 1) */
  if ((lcdrow == 1) || (lcdrow == 3))
    line = 8;
  else
    line = 12;

  /* set the command for the address set and line # */

  d = (lcdcol >> 4) | line;
  lcdout(d); /* send the high nibble	*/

  d = lcdcol & 0xf;
  lcdout(d); /* send the low nibble	*/
}
void scroll_down(int col,int row)
{
int y,x;
  row ++;
  if (row > max_row+1) {
    for (y=1;y!=max_row+1;y++) {
      /* put us at the start of the line */
      setdd(0,y);
      /* scroll using the internal lcd_buf */
      for (x=0;x!=max_col+1;x++) {
	lcd_buf[y-1][x] = lcd_buf[y][x];
	out_char_lcd(lcd_buf[y-1][x]);
      }
    }
    /* now clear the last line */
    setdd(0,max_row+1);
    for (x=0;x!=max_col+1;x++) {
      lcd_buf[max_row][x] = ' ';
      out_char_lcd(lcd_buf[max_row][x]);
    }
    /* set to the column we were on before */
    setdd(col,max_row+1);
  }
  else setdd(col,row);
}

void clear_lcd()
{
int j,i;

  /* clear internal buffer */
  for (j=0;j!=max_row+1;j++)
    for (i=0;i!=max_col+1;i++)
      lcd_buf[j][i] = ' ';

  enable_lg = 0;
  lcdout(0);
  lcdout(1);
  delay(2);	    // minimum 1.64ms
  enable_lg = 0x40; /* turn cursor off on 2nd line group */
  cursor_ctrl(0);
  lcdDelay();

  cur_row = 1;
  cur_col = 0;
  enable_lg = 0x80; /* 1st line group */
  cursor_ctrl(cur_on);
  lcdDelay();
}
//
// downloads a backslash character into character 0 of lcd
// to be used instead of "yen" symbol
//
void define_slash_char()
{
  lcdout(4);	// set to change character for 0
  lcdout(0);

  out_char_lcd(0x00); // top row     5 bits
  out_char_lcd(0x10); // second row  5 bits
  out_char_lcd(0x08);
  out_char_lcd(0x04);
  out_char_lcd(0x02);
  out_char_lcd(0x01);
  out_char_lcd(0x00); // bottom row  5 bits
}

void change_slash(void)
{
  setdd(0,0);          // set to controller for lines 0-1
  define_slash_char(); // define \ as char 0
  setdd(0,3);          // set to controller for lines 2-3
  define_slash_char(); // define \ as char 0
  setdd(0,0);          // move back to col 0, row 0
}

/*
  lcd char filter filters special characters/sequences of characters which
  would normally be used on a VF display to work the same on the LCD display.
*/
void lcd_char_filter(unsigned AL)
{
int row,col;

  row = cur_row;
  col = cur_col;

  /* check for sequence 1b xx where xx = cursor position */
  /* rrcc cccc  where rr = row 00-03, cc cccc = column */
  if (last_prn_char == 0x1b) {
    /* move cursor */
    row = ((AL & 0xc0) >> 6)+1;
    col = (AL & 0x3f);
    setdd(col,row);
    last_prn_char = 0;
  }
  else
    /* check for sequence 17 xx where xx = 1 for blinking cursor */
    if (last_prn_char == 0x17) {
      /* change cursor type */
      blinking = (AL != 0);
      if (cur_on)
	cursor_ctrl(1);
      last_prn_char = 0;
    }
  else {
    /* doing all 1 character sequences or the 1st character of
       a 2 character sequence */
    switch (AL) {
     case 8: /* move cursor left */
       if (col > 0) {
	 col --;
	 setdd(col,row);
       }
       break;
     case 9: /* advance cursor right */
       if (col < max_col) {
	 col ++;
	 setdd(col,row);
       }
       break;
     case 0xa: /* line feed */
       scroll_down(col,row);
       break;
     case 0x0d: /* cr */
       setdd(0,row);
       break;
     case 0xe: /* make cursor invisible */
       cur_on = 0;
       cursor_ctrl(cur_on);
       break;
     case 0xf: /* make cursor visible */
       cur_on = 1;
       cursor_ctrl(cur_on);
       break;
     case 0x15: /* clear display */
       clear_lcd();
       break;
     case 0x16: /* home cursor, do not clear display */
       setdd(0,1);
       break;
     case 0x1b: /* move cursor (2 byte command) */
       break;
     case 0x17: /* set cursor blinking (2 byte command) */
       break;
     default:
       if (AL == 0x5c) AL = 0; // use character 0 if backslash wanted.
       out_char_lcd(AL);
       lcd_buf[row-1][col] = AL;
       cur_col = ++col;
       if (col > max_col)
	 if (Scrolling)
	   scroll_down(0,row); // move to next line, scrolling if needed.
	 else
	   setdd(0,row); /* set cursor to beginning of current line */
    }
    last_prn_char = AL;
  }
}


void vf_out (unsigned int ochar)
{
unsigned int n;
  if (display_type == 1)
  {
    /* output the character to the vf display */
    outportb(dev_add,ochar & 0x7f);
    delay1();
    outportb(dev_add,ochar | 0x80);
    delay1();
    outportb(dev_add,ochar & 0x7f);
    delay1();
  }
  else /* type 2 */
  {
    /* output the character to the vf display */
    outportb(dev_add,ochar & 0x7f);
    delay(1);
    outportb(dev_add,ochar | 0x80);
    delay(1);
  }
}

void move_vf_cursor (unsigned int col, unsigned int row)
{
unsigned int ochar;

  vf_out (0x1b);

  if (display_type == 1)
    ochar = (row * (max_col+1)) + col;
  else { /* display_type == 2 */
    ochar = (row << 5) | (col & 0x1f);
    if (col & 0x20)
      ochar = ochar | 0x80;
  }

  cur_row = row;
  cur_col = col;

  vf_out(ochar);

}


/*
  Vf char filter filters special characters/sequences of characters for
  work for the Vf display.
*/
void vf_char_filter(unsigned AL)
{
int row,col;
unsigned int ochar,n;

  row = cur_row;
  col = cur_col;

  /* check for sequence 1b xx where xx = cursor position */
  /* rrcc cccc  where rr = row 00-03, cc cccc = column */
  /* vf 2x20 or 4x20 display */
  if (last_prn_char == 0x1b) {
    /* extract row/column from command */
    row = (AL & 0xc0) >> 6;
    if (row > max_row) row = max_row;
    col = (AL & 0x3f);
    if (col > max_col) col = max_col;

    move_vf_cursor(col,row);

    last_prn_char = 0;
  }
  else {
    /* doing all 1 character sequences or the 1st character of
       a 2 character sequence */
    ochar = AL;
    switch (AL) {
     case 8: /* move cursor left */
       if (cur_col > 0) {
	 cur_col --;
	 vf_out(ochar);
       }
       break;
     case 9: /* advance cursor right */
       if (cur_col < (max_col - 1)) {
	 cur_col ++;
	 vf_out(ochar);
       }
       break;
     case 0xa: /* line feed */
       vf_out(ochar);
       delay1(); // delay a little for line feeds

       row ++;
       if (row > max_row)
	 row = max_row;
       move_vf_cursor(col,row);
       break;
     case 0x0d: /* cr */
       cur_col = 0;
       vf_out(ochar);
       break;
     case 0xe: /* make cursor invisible */
     case 0xf: /* make cursor visible */
       if (display_type == 2)
	 vf_out(ochar);
     case 0x15: /* clear display */
     case 0x16: /* home cursor, do not clear display */
       cur_row = 0;
       cur_col = 0;
       vf_out(ochar);
       if (ochar==0x15)
	 /* delay a little longer here on clear for fast systems */
	 delay(1);
       break;
     case 0x1b: /* move cursor (2 byte command) */
       break;
     case 0x17: /* set cursor blinking (2 byte command) */
       /* not available for vf displays */
       break;
     default:
       vf_out(ochar);
       cur_col ++;
       if (cur_col > max_col)
	 if (Scrolling)
	 {
	   cur_col = 0;
	   cur_row ++;
	   if (cur_row > max_row)
	     cur_row = max_row;
	 }
	 else
	   move_vf_cursor(0,cur_row);
    }
    last_prn_char = AL;
  }
}


int clear_display(void)
{
unsigned int n;
  if ((display_type == 1) || (display_type == 2)) {
    /* VF display clear */
    outportb(dev_add,0x1b); /* esc */
    outportb(dev_add,0x95); /* 15 | strobe */
    delay(1);
    cur_row = 0;
    cur_col = 0;
    return (1);
  }
  else
    if ((display_type >= 3) && (display_type <= 6)){
      /* clear the LCD display */
      clear_lcd();
      return (1);
    }
    else return (0);
}


/* Interrupt 17 routine which when installed provides a method of
   interrogating the keypad scan data values.
   Function  Subfunction  Description

   For all functions:
     DX = line printer number installed as ( 0 = LPT1, 1 = LPT2, 2 = LPT3 )

DISPLAY functions
     AH = 0   AL = n      prints a character to the display (if one active)
			  if AL = esc then if the next character is:
			     8:  backspace cursor 1 char (don't erase)
			     9:  advance cursor 1 char (don't erase)
			     A:  Line feed, go to next line, scroll if on
				 last line and blank next line
			     D:  Carriage return return cursor to beg of line
			     E:  Make cursor invisible
			     F:  Make cursor visible
			     15: clear display position cursor top left
			     16: Home cursor, but don't clear display
			     17 xx: blinking cursor control = 1 blink.
			     1B xx: move cursor to xx, top left is 00

     AH = 0   AL = 1      initialize display (if one active) or initialize
			  keypad buffer and other variables.

     AH = 3   AL = 9      Sets scrolling flag on displays
			     BX = 0 disable scrolling
			     BX = 1 enable scrolling
KEYPAD functions
     AH = 3   AL = 0	  returns AH = number of keys in buffer.
     AH = 3   AL = 1	  returns AH = 1-16 keypad scan value in
				row column order. Or AH = 0 key not avail.
     AH = 3   AL = 2	  returns AH = currently pressed key.
			    Note: This key may or may not be added to the
			      keypad buffer depending on the current
			      debounce value and how many characters are
			      in the keypad buffer.
     AH = 3   AL = 3      Flush keypad buffer
     AH = 3   AL = 4      Sets debounce value from BX. Default 1
     AH = 3   AL = 5      Sets repeat flag. BX != 0 repeat allowed.
     AH = 3   AL = 11     Sets keypad scanning enable.
			  BX = 0 disables scanning
			  BX != 0 enables scanning

COMMON functions
     AH = 3   AL = 6	  returns AX,BX,CX = 1,2,3 show its installed.
     AH = 3   AL = 7      Returns AH = display type
			     0 = no display
			     1 = 2x20 VF display
			     2 = 4x20 VF display
			     3 = 4x40 LCD display
			     4 = 4x20 LCD display
			     5 = 2x40 LCD display
			     6 = 2x20 LCD display
			  AL = keypad type.
			     0 = no keypad.
			     1 = 4x4 keypad
			     2 = 4x8 keypad
     AH = 3   AL = 8      Returns what name the display can be accessed with
			     AX = LPTn to access display
			  Note: Make sure the display type != 0 using
			     subfunction 7!
     AH = 3   AL = 10     Uninstalls display.
			  Caution!!!
			  This restores the interrupts used by this TSR
			  but does not remove the memory used. Any TSRs
			  installed after the installation of this TSR will
			  no longer function properly if they installed
			  into INT 1C (which is very common!) or INT 17.
*/

void interrupt int_17(...)
{
  unsigned int AH,AL;
  unsigned int AXSave, BXSave, CXSave, DXSave;
  AXSave = _AX;
  BXSave = _BX;
  CXSave = _CX;
  DXSave = _DX;
  AL = AXSave & 0xff;
  AH = AXSave >> 8;	 /* extract AH and AL for tests */

  /* If the type is 3 then were accessing the keypad or getting information
     about the TSR, or if the DX matches our current line printer number
     then use our code, else pass this call onto the original INT 17 handler */

  if (DXSave == (cur_lptn-1) &&		 /* make sure its our printer AND */
      ((AH<=3) && (AH!=2)))              /* ah=0,1,3 */
  {
    enable();
    switch (AH)
    {
      case 0:
	if ((display_type == 1) || (display_type == 2))
	  vf_char_filter(AL);
	else /* not VF display */
	  if ((display_type >= 3) && (display_type <= 6)) /* lcd display */
	    lcd_char_filter(AL);
	break;
      case 1:
	if (clear_display())
	  AXSave = AL;
	else
	  AXSave = (0x800 | AL); /* I/O error */
	break;
      case 3:
	switch (AL)
	{
#if enable_keypad
	  case 0: // check if key has been pressed
	    AXSave = (AL | (key_count << 8));
	    break;
	  case 1:
	    if (key_count == 0)
	      AXSave = AL;
	    else // get which key was pressed
	    {
	      AXSave = (AL | (key_buf[key_buf_first] << 8));
	      key_buf_first ++;
	      key_count --;
	      if (key_buf_first == key_buf_len)
		key_buf_first = 0;
	    }
	    break;
	  case 2: // is a key down?
	    if (key_deb_count)
	      AXSave = (AL | (pkpc << 8));
	    else
	      AXSave = AL; // no key pressed
	    break;
	  case 3: // flush keypad buffer
	    key_count = 0;
	    key_buf_first = 0;
	    key_buf_last = 0;
	    break;
	  case 4: // set debounce constant
	    key_debounce = BXSave & 0xff;
	    break;
	  case 5: // set repeat flag
	    key_repeat = BXSave;
	    break;
#endif
	  case 6: // return am alive and installed
	    AXSave = 1;
	    BXSave = 2;
	    CXSave = 3;
	    break;
	  case 7: // return type of display and keypad
	    AXSave = ((display_type << 8) | keypad_type);
	    break;
	  case 8: // return lpt number in use
	    AXSave = cur_lptn-1;
	    break;
	  case 9: // set scrolling flag
	    Scrolling = BXSave & 1;
	    break;
	  case 10: // uninstall option (doesn't free up memory used)
	    disable();
	    if (keypad_type)
	      setvect(itr_1c,old_1c_vec);
	    setvect(itr_17,old_17_vec);
	    enable();
	    break;
#if enable_keypad
	  case 11: // set keypad scanning enable/disable
	    keypadScanning = BXSave;
	    break;
#endif
	  default:
	    break;
	} // switch (AL)
	break; // switch case AH == 3

      default: // not AH == 0..3
      break;
    } // switch AH
   asm {
     mov ax,AXSave
     mov [bp+0x10],ax
     mov ax,BXSave
     mov [bp+0x0e],ax
     mov ax,CXSave
     mov [bp+0x0c],ax
     mov ax,DXSave
     mov [bp+0x0a],ax
   }
  } // if ah == 3 or our line printer
  else // not ah == 3 or our line printer
  {
    asm {
      push ds      	        // save old DS

      pushf			// push on current flags for iret from old 17
      mov	bx,cs
      push	bx              // push return segment
      mov	bx,offset iretr
      push	bx              // push return offset

      pushf		        // push current flags (cannot do original)
      mov	bx,offset old_17_vec
      mov	ax,[bx+2]       // push segment
      push	ax
      mov	ax,[bx]         // push offset
      push	ax

      mov	ax,[bp+0x10]	// restore registers passed to us
      mov	bx,[bp+0x0e]
      mov	cx,[bp+0x0c]
      mov	dx,[bp+0x0a]
      mov	es,[bp+0x08]
      mov	ds,[bp+0x06]
      mov	si,[bp+0x04]
      mov	di,[bp+0x02]

      iret             		// call previous int 17
iretr:
      mov	[bp+0x10],ax	// save return registers on stack for return
      mov	[bp+0x0e],bx
      mov	[bp+0x0c],cx
      mov	[bp+0x0a],dx
      mov	[bp+0x08],es
      mov	[bp+0x06],ds
      mov	[bp+0x04],si
      mov	[bp+0x02],di

      pushf			// save returned flags
      pop	ds
      mov [bp+0x16],ds          // into return area
      pop	ds		// restore C data segment
    }
  } // if ah = 3 or our line printer
}

/*********************************************

  Transient portion of the code begins here

**********************************************/
void lcd_init()
{
int i,j;
unsigned char lcd_init_data[] =       {3,3,3,2,2,8,0,12,0,6};
/*				       ----------- ---- --- --- ---
  set to 8 bits wide 3 times then           |       |    |   |   |
  set function set to 4 bits wide.----------|       |    |   |   |
  2 line display, 5x7 dots			    |    |   |   |
						    |    |   |   |
	*display on, cursor off, cursor no blink----|    |   |   |
							 |   |   |
	clear display (requires about 1.6 ms.)-----------|   |   |
	entry mode set (inc, w/o shift)----------------------|   |
	clear display -------------------------------------------|

	* = The cursor has 3 possible conditions: Off, on(underline) and
	blink.  Change the second display control data as follows:

		12 = display on, no cursor
		13 = display on, blinking cursor
		14 = display on, cursor on, no blinking cursor
		15 = display on, cursor on, blinking cursor

	A blinking cursor is a solid block
*/
  enable_lg = 0; /* set to init both line groups at once */

  delay(15); // make sure 15 ms after Vcc rises to 4.5v

  /* set up the display parameters */
  for (j=0;j!=sizeof(lcd_init_data);j++)
  {
    lcdout(lcd_init_data[j]);
    delay(4);
    delay1();
  }

  cur_on = 1;
  blinking = 0;

  change_slash(); // download backslash character to character 0.

  clear_lcd();

}

#if helpmenu
int anykey()
{
int key;
  printf("\n    Press any key to continue");
  while (bioskey(1) == 0);  // wait for a key
  key = bioskey(0) & 0xff; // eat the key
  printf("\n");
  return ((key == 0x3) || (key == 0x1b));
}
#endif

void dishelp()
{
#if helpmenu
  printf("DISPLAY TSR V4.2 Help Menu.\n");
  printf("Help requested or no parameters on command line. Format is:\n");
  printf("  DISPLAY [display and keypad options] [LPTn or Address] \n");
  printf("    option    Description\n");
  printf("     D1       2x20 VF display\n");
  printf("     D2       4x20 VF display\n");
  printf("     D3       4x40 LCD display\n");
  printf("     D4       4x20 LCD display\n");
  printf("     D5       2x40 LCD display\n");
  printf("     D6       2x20 LCD display\n");
  printf("     K1       4x4 keypad\n");
  printf("     K2       4x8 keypad (Can ONLY be used with a digital I/O port)\n");
  printf("     Ln       LPT number for device; Used to indicate which line\n");
  printf("              printer (if any) the interface card is connected to.");
  printf("(default 1)\n");
  printf("     Axxxx    Hex address of device; Used when the interface card is\n");
  printf("              connected to a 8255 type digital I/O port instead\n");
  printf("              of a line printer port.\n");
  printf("     Zxxxx    Hex address of device; Used when the interface card is\n");
  printf("              connected to an Octagon EZIO digital I/O port instead\n");
  printf("              of a line printer port.\n");
  printf("     R        Allow re-install for more than 1 display or keypad.\n");
  printf("     S        Auto scrolling disable (default is scrolling enabled).\n");
  anykey();
  printf(" For example\n");
  printf("   DISPLAY D3        4x40 LCD display using LPT1.\n");
  printf("   DISPLAY D1 K1 L2  2x20 VF display & 4x4 keypad using LPT2.\n");
  printf("   DISPLAY K2 A100   4x8 keypad connected to digital I/O port at\n");
  printf("                     address 100 Hex.\n");
  printf("   DISPLAY D2 K1     4x20 VF display & 4x4 keypad using LPT1.\n");
#endif
}


#if (longmsgs == 0)
void badparameter (void)
{
  printf("Bad parameter.\n");
}
#endif

void argoutofrange (int startarg, int endarg)
{
#if longmsgs
   printf ("Argument out of range, must be from %d to %d\n",startarg,
	   endarg);
#else
   badparameter();
#endif
}

void invcharin (char *msg)
{
#if longmsgs
   printf ("Invalid character in %s\n",msg);
#else
   badparameter();
#endif
}

void invcharinadd (void)
{
#if longmsgs
   invcharin("address for display");
#else
   badparameter();
#endif
}

int isEZIO(unsigned int baseAddress)
{
  return ezioFlag;
}

void main(int argc, char *argv[])
{
int cur_param,i;
char cmd, reinstall = 0;
int plen;

  determineSpeed ();

  for (i=0;i<2;i++)
   lpt_ports[i] = peek(0x40,0x8+(i*2));  /* read in addresses of lpts */
  num_lpts = (peek(0x40,0x10) & 0xc000) >> 14; /* get how many are defined */
  dev_add = 0; /* set values to 0 */
  cur_lptn = 0;

  if (argc < 2) {
    dishelp();
    return;
  }

  cur_param = 1;
  char *parameter;
  char c;

  while ((parameter = argv[cur_param]) != NULL) {
    switch ((c=toupper(*parameter))) {
      case 'R': /* allow reinstallation for 2nd display */
	reinstall = 1;
	break;

      case 'S': /* scrolling disable */
	Scrolling = 0;
	break;

      case 'H': /* display help */
      case '?':
	dishelp();
	return;

      case 'L': /* assign LPT number */
	parameter++;
	if (*parameter < '1' || *parameter > '3') {
	  argoutofrange(1,3);
	  return;
	}
	else { /* extract lpt number */
	  cur_lptn = *parameter - '0'; /* extract lpt number */
	  /* will verify lpt exists after processing command line */
	}
	break; /* 'L'-'l' option */

      case 'Z':
      case 'A':
	/* They are specifying a display address */
	ezioFlag = (c == 'A') ? 0 : 1;
	char* p = parameter;
	plen = strlen(p);

	for (i = 0;i < plen-1;i++)
	  if (!isxdigit(*++p)) {
	    invcharinadd();
	    return;
	  }
	if (!sscanf(++parameter,"%x",&dev_add)) {
	  invcharinadd();
	  return;
	}
	if (dev_add == 0) {
#if longmsgs
	  printf("Zero is not allowed for Axxx.\n");
#else
	  badparameter();
#endif
	  return;
	}
	break; /* 'A'-'a' option */

      case 'D':
	parameter++;
	if (*parameter < '1' || *parameter > '6') {
	  argoutofrange(1,6);
	  return;
	}
	else {
	  display_type = *parameter - '0';
	  switch (display_type) {
	  case 1: max_col = 19; max_row = 1; break;
	  case 2: max_col = 19; max_row = 3; break;
	  case 3: max_col = 39; max_row = 3; break;
	  case 4: max_col = 19; max_row = 3; break;
	  case 5: max_col = 39; max_row = 1; break;
	  case 6: max_col = 19; max_row = 1; break;
	  default: break;
	  }
	} /* else n of Dn is valid */
	break;

#if enable_keypad
      case 'K':
	parameter++;
	if (*parameter < '1' || *parameter > maxkeypad) {
	  argoutofrange(1,maxkeypad - '0');
	  return;
	}
	else {
	  keypad_type = *parameter - '0';
	}
	break;
#endif
      default:
#if longmsgs
#if enable_keypad
	printf("Unknown switch on command line must be K/D/L/A/R/S/?\n");
#else
	printf("Unknown switch on command line must be D/L/A/R/S/?\n");
#endif
#else
	badparameter();
#endif
	return;
    } /* end of switch */
    cur_param ++;
  } /* end of while */

  for (i=0;i<3;i++)
  {
    _AH = 3;
    _AL = 6;
    _DX = i;
    geninterrupt(itr_17);
    if ((_AX == 1) && (_BX == 2) && (_CX == 3))
    {
      if (!reinstall)
      {
#if longmsgs
	printf("DISPLAY TSR previously installed.\n");
	printf("Use the R switch when installing another display or keypad.\n");
#else
	badparameter();
#endif
	return;
      }
    }
  }

  /* Now verify options are correct */
  /* first did they specify both Ln and Axxxx */
  if ((cur_lptn != 0) && (dev_add != 0)) {
#if longmsgs
    printf("Both Ln and Axxxx options specified, please specify only one.\n");
#else
    badparameter();
#endif
    return;
  }

  /* verify they specified a device */
  if ((display_type == 0 ) && (keypad_type == 0)) {
#if longmsgs
    printf("You didn't specify a display or a keypad type.\n");
#else
    badparameter();
#endif
    return;
  }

  /* If no L or A specified then use LPT1, if available */
  if ((cur_lptn == 0) && (dev_add == 0)) {
    if (num_lpts == 0) {
#if longmsgs
      printf("LPT1 does not exist.\n");
#else
      badparameter();
#endif
      return;
    }
    cur_lptn = 1;
    using_lpt = 1;
  }

  /* decide if lpt is defined */
  if (dev_add != 0) {
    i = 0;
    /* find if address specified is in the current lpt list */
    while ((lpt_ports[i] != dev_add) && (i < 3)) i++;
    /* if we didn't find it then see if we can add it */
    if (i == 3) /* not found */
      if (num_lpts == 3) { /* already three line printers */
#if longmsgs
	printf("Three line printers already defined, cannot add any more.\n");
#else
	badparameter();
#endif
	return;
	}
      else { /* we aren't up to three line printers...we can add it */
	poke(0x40,0x8+((num_lpts)*2),dev_add); /* add address to lpt list */
	/* set new number of printers in equipment flag */
	/* NOTE: DOS may not allow access as LPTx after assigning this!
	   However you can still access it using bios interrupts in
	   quick basic or C... */
	cur_lptn = num_lpts + 1; /* use next lpt number */
	poke(0x40,0x10,((peek(0x40,0x10) & 0x3fff) | (cur_lptn << 14)));
	using_lpt = 0;
      }
    else { /* they specified an address of an existing lpt */
      cur_lptn = i;
      using_lpt = 1;
    }
  }
  else { /* they should have specifed an existing LPT */
    if (cur_lptn > num_lpts) {
#if longmsgs
      printf ("LPT%01d not defined.\n",cur_lptn);
#else
      badparameter();
#endif
      return;
    }
    else {
      dev_add = lpt_ports[cur_lptn-1]; /* assign address of lpt */
      using_lpt = 1;
    }
  }

#if enable_keypad
  /* verify 4x8 keypad not specified for lpt port */
  if ((keypad_type == 2) && (using_lpt)) {
#if longmsgs
    printf("4x8 keypad will only work when connected to a digital I/O port.\n");
#else
    badparameter();
#endif
    return;
  }

#endif

  if (using_lpt == 0)
  {
    if(isEZIO(dev_add))
    {
      if (keypad_type == 2)
      {
	outportb(dev_add+3,EZDIR_CMD);  // set direction command
	outportb(dev_add,0xff);         // A port all out
	outportb(dev_add+1,0xff);	// B port all out
	outportb(dev_add+2,0x00);	// C port all in
	outportb(dev_add+3,EZ8255_CMD); // 8255 implementation command
	outportb(dev_add,0);            // A port init data
	outportb(dev_add+1,0);		// B port init data
	outportb(dev_add+2,0);		// C port init data
      }
      else // 4x4 keypad
      {
	outportb(dev_add+3,EZDIR_CMD);  // set direction command
	outportb(dev_add,0xff);         // A port all out
	outportb(dev_add+1,0xff);	// B port all out
	outportb(dev_add+2,0xf0);	// C port 4 out(upper 4),4 in(lower 4)
	outportb(dev_add+3,EZ8255_CMD); // 8255 implementation command
	outportb(dev_add,0);            // A port init data
	outportb(dev_add+1,0);		// B port init data
	outportb(dev_add+2,0);		// C port init data
      }
    }
    else /* configure 8255 ports */
    {
      if (keypad_type == 2)
	outportb(dev_add+3,0x89); /* A/B out, C lower & upper in */
      else /* 4x4 keypad */
	outportb(dev_add+3,0x81); /* A/B/C upper out, C lower in */
    }
  }

  /* Print sign on message and instructions */
  printf("Installing DISPLAY TSR V4.3 interrupt routine.\n");

#if longmsgs
  if (display_type)
  {
    printf ("%dx%d ",max_row+1, max_col+1);
    if ((display_type >=1) && (display_type <=2))
      printf("VF");
    else
      printf("LCD");

    printf (" display");
  }

#if enable_keypad
  if ((display_type != 0) && (keypad_type != 0)) printf(" & ");

  switch (keypad_type) {
    case 1: printf ("4x4 keypad"); break;
    case 2: printf ("4x8 keypad"); break;
    default: break;
  }
#endif

  if (using_lpt)
    printf(" connected to LPT%01d.\n",cur_lptn);
  else {
    printf(" installed at address %04xH\n",dev_add);
    printf("use INT 17H to access.\n");
  }
#endif // longmsgs

  /* clear the display on start up (before anyone can interrupt us)*/
  if ((display_type >= 3) && (display_type <= 6))
    lcd_init();

  if (display_type != 0)
    clear_display();

#if enable_keypad
  /* Get & save old interrupt vector, install new vector */
  if (keypad_type != 0) {
    old_1c_vec = getvect(itr_1c);
    setvect(itr_1c,keypad_scan);
  }
#endif

  /* install int 17 for either the display or keypad */
  old_17_vec = getvect(itr_17);

  setvect(itr_17,int_17);

  // the following is taken from the keep example (32 = safety)
  // and should automatically determine correct size to keep.
  keep(0, (_SS + ((_SP + 32)/16) - _psp));
}

