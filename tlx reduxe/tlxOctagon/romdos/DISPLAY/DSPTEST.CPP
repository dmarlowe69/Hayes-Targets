 /*

        This software, including source code, is provided at no cost.  It
        is intended to be used for tutorial and demo purposes, and therefore,
        no warranty of any kind, express or implied is provided.  Under no
        circumstances whatsoever shall Octagon Systems Corporation be held
        liable for any damages, including any lost profits, lost savings,
        or other incidental or consequential damages arising from the use
        of, or inablity to use, this software.

       Octagon Systems Corporation (TM)
       6510 W. 91st. Ave.
       Westminster, CO  80030


	Filename:		DSPTEST.CPP

	Function name:		main

	Received parameters:	none

	Returned parameters:	none

	Edit date:		9-02-94

	Function:		Tests the DISPLAY TSR display support.

	Revisions:

   02-21-92 EH Add support for 4x20 LCD v2.0
   03-05-92 EH Add support for 2x40 LCD v2.1
   11-25-92 EH Add support for 2x20 LCD, convert to CPP v3.5.0
   09-02-94 EH Add support for new display (v4.0) v4.0

*/

/* Includes */

#include	<stdio.h>
#include	<dos.h>
#include	<bios.h>
#include	<stdlib.h>
#include	<string.h>
#include	<conio.h>

#define itr_17	0x17
#define esc 0x1b
#define cr 13
#define lf 10
int lptnum;
int display_type;
int max_row;
int max_col;

void move_row_col (int row, int col)
{
  /* Error check col, row */
  if (col > max_col) col = max_col;
  if (row > max_row) row = max_row;

  biosprint(0,esc,lptnum);
  biosprint(0,((row << 6) | col),lptnum);
}

void print_dsp (char *msg)
{
int i;

  for (i = 0; i < strlen (msg) ; i ++ )
    biosprint(0,msg[i],lptnum);

}
void set_cursor_blink (int cursor_on)
/* if cursor_on then it turns on cursor blinking, else it turns it off */
/* This only is valid on the 4x40 lcd display */
{
  if (display_type > 2) {
    biosprint(0,0x17,lptnum);
    biosprint(0,cursor_on,lptnum);
    /* Note: You cant use "\x17\x0" as a string to print_dsp since
       the \x0 is seen as the string terminator and is not sent out. */
  }
}

int anykey()
{
  int key;

  printf("Press any key to continue");
  /* go wait for a key */
  while (bioskey(1) == 0);  // wait for a key
  key = bioskey(0) & 0xff; // eat the key
  printf("\n");
  return ((key == 0x3) || (key == 0x1b));
}


int printdisplay(int lptnum)
{
int display_type;

    _AH = 3;
    _AL = 7;
    _DX = lptnum;
    geninterrupt(itr_17);
    display_type = _AH;

    switch (display_type)
    {
      case 1: printf ("2x20 VF"); break;
      case 2: printf ("4x20 VF"); break;
      case 3: printf ("4x40 LCD"); break;
      case 4: printf ("4x20 LCD"); break;
      case 5: printf ("2x40 LCD"); break;
      case 6: printf ("2x20 LCD"); break;
      default: break;
    }
    printf(" connected to LPT%d.\n",lptnum+1);
    return (display_type);
}

void main ()
{
char string[17];
int i,j, key;
int numdisplays = 0,dpt;
int displaysinstalled [] = {0,0,0};
int typedisplays [] = {0,0,0};

  /* Print sign on message and instructions */
  printf("DISPLAY TSR display demonstration program V4.0\n");

  /* determine if there are any displays installed using display TSR */
  for (i = 0; i < 3; i++)
  {
    _AH = 3;
    _AL = 6;
    _DX = i;
    geninterrupt(itr_17);
    if ((_AX == 1) && (_BX == 2) && (_CX == 3))
    { // display tsr installed for this lpt
      dpt = printdisplay(i); // go check/print type of display installed (if any)
      if (dpt)
      {
	display_type = dpt;
	numdisplays++;
	displaysinstalled[i] = 1;
	typedisplays[i] = display_type;
	lptnum = i;
      }
    }
  }

  /* give option for which display if more than one exists */
  switch (numdisplays) {
  case 0:
    printf ("\nDISPLAY TSR is not installed or no display options used!\n");
    printf ("Install DISPLAY using DISPLAY Dn.\n");
    return;
  case 1:
    break;
  default:
    key = 0;
    while (!((key >= '1') && (key <= '3')))
    {
      printf("Which LPT is the test display assigned to?");
      while (!kbhit());
      key = bioskey(0) & 0x7f; // get the key
      if ((key == 0x1b) || (key == 0x03))
	return;
      if (displaysinstalled[key-'1'] == 0)
      {
	printf("\nNo display connected to LPT%d\n",key-'0');
	key = 0;
      }
    }
    display_type = typedisplays[key-'1'];
    lptnum = key-'1';
    printf("\nTesting display connected to LPT%d.\n",lptnum+1);
  }

  switch (display_type)
  {
    case 1: max_col = 19; max_row = 1; break;
    case 2: max_col = 19; max_row = 3; break;
    case 3: max_col = 39; max_row = 3; break;
    case 4: max_col = 19; max_row = 3; break;
    case 5: max_col = 39; max_row = 1; break;
    case 6: max_col = 19; max_row = 1; break;
  }

  print_dsp("\x15"); /* clear the display */

  if (max_col <= 20) {
    print_dsp("   Display sample\n\r");
    print_dsp("       program");
  }
  else {
    print_dsp("         Display sample program");
  }
  if (anykey()) return;

  for (i = 0;i <= max_row; i++) {
    print_dsp("\n\r");
    print_dsp("This is line ");
    j = i;
    itoa(j,string,10);
    print_dsp(string);
    printf("This is line %d\n",j);
  }
  if (anykey()) return;
  print_dsp("\x15"); /* clear display */
  printf("Try some cursor control stuff\n");

  for (i = 0; i <= max_row; i++) {
    move_row_col ( i, i*5 );
    print_dsp("<<<");
    printf("Move cursor to position %d of line %d and print <<<\n",i*5,i);
  }
  if (anykey()) return;

  printf("Now clear the display\n");
  print_dsp("\x15");
  if (anykey()) return;

  if (display_type > 1) {
    printf("Now to change the cursor\n");
    print_dsp("Turn the cursor off\xe");
    if (anykey()) return;
    print_dsp("\n\rNow back on again\xf");
    if (anykey()) return;
    if (display_type > 2) {
      print_dsp("\n\rNow make it blink");
      set_cursor_blink(1);
      if (anykey()) return;
      print_dsp("\n\rNow back to normal");
      set_cursor_blink(0);
      if (anykey()) return;
    }
    print_dsp("\n\r\n");
  }

  printf("Move the cursor by 1 character left/right\n");
  printf("Forward\n");
  print_dsp("Forward\n\r");
  for (i = 0; i <= max_col-1; i++) {
    if (display_type == 1)
      print_dsp("*\x08"); /* print a * since theres no cursor for 2x20 vf */
    print_dsp("\x09"); /* do actual move forward */
    delay(10); /* slow things so you can see the cursor move */
  }

  printf("Backward\n");
  print_dsp("\n\rBackward\n\r");
  move_row_col (max_row,max_col);
  for (i = 0; i <= max_col - 1; i ++) {
    if (display_type == 1)
      print_dsp("\x08*"); /* print a * since theres no cursor on 2x20 vf */
    print_dsp("\x08"); /* do actual move backward */
    delay(10); /* slow things so you can see the cursor move */
  }
  if (anykey()) return;

  print_dsp("\x15");
  if (max_col <= 20)
    print_dsp("   End of DISPLAY\n\r      TSR test ");
  else
    print_dsp("       End of DISPLAY TSR test");

}