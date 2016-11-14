 /* file: scrdump.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("3D Cable Analysis")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *	Screen Dump
 *
 *	written by denton marlowe
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
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *
 *		 This Software is dedicated to the memory of
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

 #include "stdio.h"
 #include "stdlib.h"
 #include "bios.h"
 #include "dos.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #define MICROSOFTC
 #define SINGLE 0
 #define DOUBLE 1
 #define QUAD	2

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 void print(char ch);
 void set_graphics(int cols, int density);
 void reset(void);

 void print_scr(int startx, int starty, int endx, int endy,
               int pagex, int pagey, int density);

 char getpoint(int x, int y);

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

/* This function prints the specified screen region at the
   specified page location.  

   Startx,starty are the coordinates of the upper left
   corner of the region of the screen to print; endx,endy
   specify the lower right corner.

   The starting location of the page to print the image
   is specified by pagex,pagey. The horizontal page
   location is in characters, the vertical is in lines 
   (1/6").  Upper left corner of page is 0, 0.

   The graphics density is specified by the density
   parameter.
*/
void print_scr(int startx, int starty, int endx, int endy,
               int pagex, int pagey, int density)
{
  register int i, x, y, px;
  int cols, color, sum;

  endx++;  endy++;
  cols = endx - startx;

  for(; pagey>=0; pagey--) print('\n');

  for(y = starty; y<endy; y+=8) {
    for(px=0; px<pagex; px++) print(' ');
    set_graphics(cols, density);
    for(x = startx; x<endx; x++) {
      sum = 0;
      for(i=0; i<8; i++) {
        if(y+i < endy) {
          color = getpoint(x, y+i);
          if(color) sum += 1<<(7-i);
        }
      }
	  print((char)(sum));
    }
    print('\n');
  }
}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

/* Set the graphics density as specified. */
void set_graphics(int cols, int density)
{
  union {
    unsigned char c[2];
    unsigned int i;
  } u;
  char den_code;

  u.i = cols;
  
  switch(density) {
    case SINGLE: den_code = 75; 
      break;
    case DOUBLE: den_code = 76;
      break;
    case QUAD: den_code = 90;
      break;
  }   

  print(27); print(65); print(8); /* 8/72 line spacing */
  print(27); print(den_code); print(u.c[0]); print(u.c[1]);
}

/* Reset the printer to defaults. */
void reset(void)
{
  print(27); print(64);
}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

/* Send a character to the printer. */
void print(char ch)
{
#ifdef TURBOC
  biosprint(0, ch, 0);
#endif
#ifdef MICROSOFTC
  _bios_printer(0, 0, ch);
#endif
}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

/* Returns the value at the specified pixel. */
char getpoint(int x, int y)
{
  union REGS r;

  r.h.ah = 13;
  r.x.dx = y;
  r.x.cx = x;
  r.h.bh = 0;
  int86(16, &r, &r);
  return r.h.al;
}
