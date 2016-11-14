/* A Screen Editor Subsystem. */

//#define TURBOC
#define MICROSOFTC
/* If you use Microsoft C/C++, #define MICROSOFTC
   instead of TURBOC.   If you have a different
   C compiler, see the text.
*/

#include "stdio.h"
#include "dos.h"
#include "string.h"
#include "bios.h"
#include "conio.h"

#define BUF_SIZE 32000
#define LINE_LEN 79
#define MAX_LINES 24
#define KILL_BUF_SIZE 4*LINE_LEN

char buf[BUF_SIZE];
char *curloc, *endloc;
int scrnx, scrny;
char killbuf[KILL_BUF_SIZE];

char *helpline = 
  "F1: save  F2: load  F3: find  F4: replace\
   ^K: Lill line  ^Y: Yank  ^Z: quit";

void edit(char *fname), help(void);
void gotoxy(int x, int y), clrline(int y);
void edit_clr_eol(int x, int y), clrscr(void);
void printline(char *p), delete_char(void);
void search(void), kill_line(void);
void upline(void);
void downline(void), left(void),  right(void);
void scrolldn(int x, int y);
void scrollup(int topx, int topy, int endx, int endy);
void display_scrn(int x, int y, char *p);
void pagedown(void), pageup(void), replace(void);
void home(void), gotoend(void), yank(void);
int load(char *fname), save(char *fname);
void edit_gets(char *str);

//main(int argc, char *argv[])
//{
//	if(argc==2) edit(argv[1]);
//	clrscr();
//
//	return 0;
//}

/*  This is the editor's entry function and contains
    its main loop.

    Call it with the name of the file you want to edit.
*/
void edit(char *fname)
{
  union k {
    char ch[2];
    unsigned i;
  } key;
  char name[80];

  /* try to load the file */
  if(!load(fname)) curloc = endloc = buf;
  strcpy(name, fname);

  clrscr();
  
  /* set initial values to X,Y coordinate vars */
  scrnx = scrny = 0;

  /* display a screen full of text */
  display_scrn(0, 0, curloc);
  help();  /* print the help line */
  gotoxy(0, 0); 

  /* editor main loop */
  do {
#ifdef TURBOC
    key.i = bioskey(0);  /* read 16-bit key code */
#endif
#ifdef MICROSOFTC
    key.i = _bios_keybrd(0);
#endif
    if(!key.ch[0]) { /* is a command */
      switch(key.ch[1]) {
        case 59: /* F1: save file */ 
          save(name);
          break;
        case 60: /* F2: load file */
          clrline(MAX_LINES);
          gotoxy(0, MAX_LINES);
          printf("enter filename: ");
          edit_gets(name);
          if(*name) load(name);
          help();
          display_scrn(0, 0, curloc);
          scrnx = scrny = 0;
          break;
        case 61: /* F3: search */
          search();
          break;
        case 62: /* F4: replace */
          replace();
          break;
        case 71: /* home */
          home();
          break;
        case 79: /* end */
          gotoend();
          break;
        case 75: /* left */
          left();
          break;
        case 77: /* right */
          right();
          break;
        case 72: /* up  */
          upline();
          break;
        case 80: /* down */
          downline();
          break;
        case 73: /* page up */
          pageup();
          break;
        case 81: /* page down */
          pagedown();
          break;
        case 83: /* Del */
          if(curloc<endloc) delete_char();
          break;
      }
      if(curloc < buf) {
        scrnx = scrny = 0;
        curloc = buf;
      }
      gotoxy(scrnx, scrny); /* position cursor */
    }
    else { /* enter keystroke into file */
      switch(key.ch[0]) {
        case '\r':  /* carriage return */
          /* see if buffer is full */
          if(endloc==buf+BUF_SIZE-2) break;

          /* move contents of file below current
             location down one byte to make room
             for the RETURN
          */
          memmove(curloc+1, curloc, endloc-curloc+1);

          *curloc = key.ch[0]; /* put RETURN in file */
          curloc++;

          /* clear rest of line */
          edit_clr_eol(scrnx, scrny);
          scrnx = 0;
          scrny++;

          /* move text on screen down */
          if(scrny==MAX_LINES) {  /* at bottom of page */
            scrny = MAX_LINES-1;
            scrollup(0, 0, LINE_LEN, scrny);
          }
          else scrolldn(scrnx, scrny);
     
          gotoxy(scrnx, scrny);
          printline(curloc); /* display the line */
          endloc++;  /* advance the end of file pointer */
          break;
        case '\b':  /* backspace */
          if(curloc==buf) break;
          left();
          delete_char();
          break;
        case 11: /* control-K: kill line */
          kill_line();
          break;     
        case 25: /* control-Y: yank kill buffer */
          yank();
          break;
        default:
          /* see if buffer is full */
          if(endloc==buf+BUF_SIZE-2) break;
          
          /* can't type past end of line */
          if(scrnx==LINE_LEN) break;

          /* move contents of file below current
             location down one byte to make room
             for the character
          */
          memmove(curloc+1, curloc, endloc-curloc+1);
          *curloc = key.ch[0];  /* put keystroke in file */ 
          putch(*curloc);  /* display the key on the screen */
          scrnx++;  /* advance X */
          gotoxy(scrnx, scrny);
          printline(curloc+1); /* display the line */
          curloc++;  /* advance the current location */
          endloc++;  /* advance the end of file pointer */
      }
      gotoxy(scrnx, scrny);
    }
  } while(key.ch[0]!=26);  /* control-Z quits the editor */
}

/* Display a line pointed to by p.  This function
   stops when it hits a carriage return or the
   end of the file.
*/
void printline(register char *p)
{
  register int i;

  i = scrnx;
  while(*p!='\r' && *p && i<LINE_LEN) {
    putch(*p);
    p++;
    i++;
  }
}

/* Insert previously killed line. */
void yank(void)
{
  char *p;
  
  p = killbuf;
  while(*p) {  
    memmove(curloc+1, curloc, endloc-curloc+1);
    *curloc = *p;  /* put keystroke in file */ 
    if(scrnx<LINE_LEN) {
      putch(*curloc);  /* display the key on the screen */
      scrnx++;
    }
    curloc++;
    endloc++;
    p++;
  }
  printline(curloc);
}

/* Delete the line at the current location. */
void kill_line(void)
{
  register int i;
  char *p, *killbufptr;

  if(*curloc=='\r') {
    delete_char();
    return;
  }

  edit_clr_eol(scrnx, scrny);  /* clear to CR */

  /* find out how many characters are in the line */
  p = curloc;
  i = 0;
  killbufptr = killbuf;
  while(*p!='\r' && p<endloc) {
    i++;
    *killbufptr = *p;  /* put in kill buffer */
    p++;
    if(killbufptr<killbuf+KILL_BUF_SIZE-2) killbufptr++;
  }
  *killbufptr = '\0';

  /* remove the line */
  memmove(curloc, curloc+i, endloc-curloc);
  endloc -= i;
}

/* Global search and replace. */
void replace(void)
{
  register int len1;
  char str1[80], str2[80];
  char *p, *p2;

  clrline(MAX_LINES);
  gotoxy(0, MAX_LINES);
  printf("enter string to replace: ");
  edit_gets(str1);

  clrline(MAX_LINES);
  gotoxy(0, MAX_LINES);
  printf("enter replacement: ");
  edit_gets(str2);

  p = curloc;
  len1 = strlen(str1);
 
  while(*str1) {
    /* search for the string */
    while(*p && strncmp(str1, p, len1)) p++;
    if(!*p) break;  /* not found */

    /* remove old string */
    memmove(p, p+len1, endloc-p);
    endloc -= len1;

    /* insert new string */
    p2 = str2;
    while(*p2) {  
      memmove(p+1, p, endloc-p+1);
      *p = *p2;
      p++;
      endloc++;
      p2++;
    }
  }
  clrscr();

  /* find location of top of screen */
  p = curloc;
  for(len1 = scrny; len1>=0 && p>buf; ) {
    p--;
    if(*p=='\r') len1--;
  }
  if(*p=='\r') p++;
      
  /* redisplay current screen */
  display_scrn(0, 0, p);
  help();
}

/* Delete character at the current location. */
void delete_char(void)
{
  gotoxy(scrnx, scrny);

  if(*curloc=='\r') { /* RETURN, scroll display */
    scrollup(0, scrny+1, LINE_LEN, MAX_LINES-1);            
    memmove(curloc, curloc+1, endloc-curloc);
    endloc--;
    display_scrn(scrnx, scrny, curloc);
    help();
  }
  else {
    memmove(curloc, curloc+1, endloc-curloc);
    endloc--; 
    printline(curloc); printf(" ");
  }
}
 
/* Display help line.  You might want to expand on
   this idea.
*/
void help(void)
{
  gotoxy(0, MAX_LINES); 
  printf(helpline);
}

/* Move current location left. */
void left(void)
{
  if(curloc==buf) return; /* can't go left */
  scrnx--;
  if(scrnx < 0) { /* at beginning of a line */
    scrnx = 0;
    upline();  /* go up to next line */

    /* find end of line */
    while(*curloc!='\r') {
      curloc++;
      scrnx++;
    }
  }
  else curloc--;
}

/* Move current position right. */
void right(void)
{
  /* can't move right */
  if(curloc+1 > endloc) return;

  scrnx++;

  /* if at end of line, go to next one */
  if(scrnx > LINE_LEN || *curloc=='\r') {
     scrnx = 0;
     scrny++;
     if(scrny==MAX_LINES) { /* at end of screen */
       scrny = MAX_LINES-1;
       downline();  /* get next line */

       /* move cursor and current loc to start of
          new line */
       curloc--;
       while(*curloc!='\r') curloc--;
       curloc++; 
       scrnx = 0;
     }
     else curloc++;
  }
  else curloc++;
}

/* Find a string. */
void search(void)
{
  char str[80];
  register char *p;
  int len, i;
 
  clrline(MAX_LINES);  /* clear message line */
  gotoxy(0, MAX_LINES);
  printf("search string: ");
  edit_gets(str);
  if(!*str) return;

  p = curloc;
  len = strlen(str);

  /* search for the string */
  while(*p && strncmp(str, p, len)) p++;
  if(!*p) return;  /* not found */
  
  /* back up to start of line */
  i = 0;
  while(p>buf && *p!='\r') {
    p--;
    i++;
  }
  p++;
  i--;

  /* reposition current location to start of match */
  curloc = p+i;
  scrnx = i;
  scrny = 0;

  /* display screen of text at location of match */
  clrscr();
  display_scrn(0, 0, p);
  help();
}

/* Move up one line.  If possible, keep scrnx same
   as in previous line.
*/ 
void upline(void)
{
  register int i;
  char *p;

  if(curloc==buf) return;  /* at first byte in buffer */

  p = curloc;
  
  if(*p=='\r') p--; /* if at end of line, back up */

  /* back up locator to start of current line */
  for(; *p!='\r' && p>buf; p--) ;
  if(*p!='\r') return;  /* at first line, cannot go up */
  curloc = p;
  curloc--;  /* skip past CR */
  i = scrnx;  /* save X coordinate */

  /* find start of next line */
  while(*curloc!='\r' && curloc>=buf) curloc--; 
  scrny--; scrnx = 0;
  curloc++;

  /* at top of screen, must scroll up */
  if(scrny<0) {
    scrolldn(0, 0);
    scrny = 0;
    gotoxy(0, 0);
    printline(curloc);
  }

  /* position the cursor and current location at
     same scrnx position as previous line if possible */
  while(i && *curloc!='\r') {
    curloc++;
    scrnx++;
    i--;
  }
}  

/*  Move down one line.  Keep previous scrnx
    location if possible.
*/
void downline(void)
{
  register int i;
  char *p;

  i = scrnx;
  p = curloc;
  
  /* advance current location to start of next line */
  while(*p!='\r' && p<endloc) p++;
  if(p==endloc) return;  /* can't go down */
  p++;  /* skip past CR */
  curloc = p;
  scrny++; scrnx = 0;

  /* if moving down past the bottom of the screen */
  if(scrny==MAX_LINES) {
    scrny = MAX_LINES-1;
    scrollup(0, 0, LINE_LEN, MAX_LINES-1); 
    gotoxy(scrnx, scrny);
    printline(curloc);
  }

  /* advance to corresponding character in next line */
  while(i && *curloc!='\r' && curloc<endloc) {
    curloc++;
    scrnx++;
    i--;
  }
}  

/* Display a screen full of text (up to 24 lines)
   starting at the specified location.
*/
void display_scrn(int x, int y, char *p)
{
  register int i;
  gotoxy(x, y);

  i = 0;
  while(y<MAX_LINES && *p) {
    switch(*p) {
      case '\r': printf("\n");
        y++;
        i = 0;
        break;
      default: if(i<LINE_LEN) putch(*p);
        i++;
    }
    p++;
  }
}

/* Page down MAX_LINES lines. */
void pagedown(void)
{
  register int i;

  clrscr();

  /* count down MAX_LINES lines */
  for(i=0; i<MAX_LINES && curloc<endloc; ) {
    if(*curloc=='\r') i++; 
    curloc++;
  }
  help();
  scrnx=0; scrny=0;
  display_scrn(0, 0, curloc);
}

/* Page up MAX_LINES lines. */
void pageup(void)
{
  register int i;

  clrscr();
  /* if current location points to a CR,
     move current location back on position */
  if(*curloc=='\r' && curloc>buf) curloc--;

  /* go back MAX_LINES in buffer */
  for(i=0; i<MAX_LINES+1 && curloc>buf; ) {
    if(*curloc=='\r') i++; 
    curloc--;
  }

  /* if not at the top line, increment the
     current location pointer past the CR */
  if(i==MAX_LINES+1) curloc += 2;

  help();
  scrnx=0; scrny=0;
  display_scrn(0, 0, curloc);
}

/* Go to the top of the file. */
void home(void)
{
  clrscr();
  curloc = buf;
  scrnx = scrny = 0;
  display_scrn(0, 0, curloc);
  help();
}

/* Go to the end of the file. */
void gotoend(void)
{
  clrscr();
  curloc = endloc;
  pageup();
}

/* Load a file. */
load(char *fname)
{
  FILE *fp;
  char ch, *p;

  if((fp = fopen(fname, "rb"))==NULL) 
    return 0;  

  p = buf;
  while(!feof(fp) && p!=buf+BUF_SIZE-2) {
    ch = getc(fp); 
    if(ch!='\n' && ch!=EOF) {
      *p = ch;
      p++;
    }
  }
  *p = '\0';
  fclose(fp);
  curloc = buf;
  endloc = p;
  return 1;
}

/* Save a file. */
save(char *fname)
{
  FILE *fp;
  char *p, name[80];

  if(!*fname) {
    printf("filename: ");
    gets(name);
  }
  else strcpy(name, fname);
  
  if((fp = fopen(name, "wb"))==NULL) 
    return 0;  

  p = buf;
  while(p!=endloc) {
    if(*p!='\r')
      putc(*p, fp);
    else {
      putc('\r', fp);
      putc('\n', fp);
    }
    p++;
  }
  fclose(fp);
  return 1;
}

/* Read a string from the keyboard, but do not
   scroll the display when a RETURN is entered.
*/
void edit_gets(char *str)
{
  char *p;

  p = str;

  for(;;) {
    *str = getch();
    if(*str=='\r') { /* return when RETURN entered */
      *str = '\0';  /* NULL terminate */
      return;
    }

    if(*str=='\b') {  /* backspace */
      if(str>p) {
        str--;
        putch('\b');
        putch(' ');
        putch('\b');
      }
    }
    else {
      putch(*str);
      str++;
    }
  }
}

/* Read and save cursor coordinates. */
void cursor_pos(void)
{
  union REGS i, o;

  i.h.bh = 0;
  i.h.ah = 3;
  int86(16, &i, &o);
}

/* Send cursor to specified X,Y (0,0 is upper
   left corner). */
void gotoxy(int x, int y)
{
  union REGS i;

  i.h.dh = y;
  i.h.dl = x;
  i.h.ah = 2;
  i.h.bh = 0;
  int86(16, &i, &i);
}

/* Clear entire line given its Y coordinate. */
void clrline(int y)
{ 
  register int i;

  gotoxy(0, y);
  for(i=0; i<LINE_LEN; i++) putch(' ');
}

/* Clear to end of specified line.  This function
   is for use with the editor only because it clears
   a line up to a carriage return. */
void edit_clr_eol(int x, int y)
{
   char *p;
  
   p = curloc;
   gotoxy(x, y);
   for(; x<LINE_LEN && *p!='\r' && *p; x++, p++) {
     printf(" ");
   }
}

/* Clear the screen. */
void clrscr(void)
{  
  union REGS r;

  r.h.ah = 6;
  r.h.al = 0;
  r.h.ch = 0;
  r.h.cl = 0;
  r.h.dh = MAX_LINES;
  r.h.dl = LINE_LEN;
  r.h.bh = 7;
  int86(0x10, &r, &r);
}

/* Scroll down the screen. This function scrolls 
   all but the bottom line. */
void scrolldn(int x, int y)
{  
  union REGS r;

  r.h.ah = 7;
  r.h.al = 1;
  r.h.ch = y;
  r.h.cl = x;
  r.h.dh = MAX_LINES-1;
  r.h.dl = LINE_LEN;
  r.h.bh = 7;
  int86(0x10, &r, &r);
}

/* Scroll up the screen using the specified
   coordinates. */
void scrollup(int topx, int topy, int endx, int endy)
{  
  union REGS r;

  r.h.ah = 6;
  r.h.al = 1;
  r.h.ch = topy;
  r.h.cl = topx;
  r.h.dh = endy;
  r.h.dl = endx;
  r.h.bh = 7;
  int86(0x10, &r, &r);
}
