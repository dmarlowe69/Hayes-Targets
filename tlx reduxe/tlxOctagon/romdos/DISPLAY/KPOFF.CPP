/*

	This software, including source code, is provided as is. It has been
	tested under a variety of conditions but Octagon Systems Corporation (TM)
	makes no warranty of any kind, express or implied for its specific
	use with your application. Under no circumstances whatsoever
	shall Octagon Systems Corporation (TM) be held liable for any damages,
	including any lost profits, lost savings, or other incidental or
	consequential damages arising from the use of, or inablity to use,
	this software.

  Filename:		KPOFF.cpp

  Function:		Disables keypad scanning in display v4.0

  Revisions:
  09-09-94   EH   New module

*/
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <bios.h>

#define itr_17 0x17

int keypad_type = 0;
int lptnum = 0;

void KeypadScan(int scan)
{
    _AH = 3;
    _AL = 11;
    _BX = scan; // 0 = disable 1 = enable
    _DX = lptnum;
    geninterrupt(itr_17);
}

int printkeypad(int lptnum)
{
int keypad_type;
    _AH = 3;
    _AL = 7;
    _DX = lptnum;
    geninterrupt(itr_17);
    keypad_type = _AL;
    switch (keypad_type)
    {
      case 1:
	printf("4x4 Keypad assigned to LPT%d.\n",lptnum+1);
	break;
      case 2:
	printf("4x8 Keypad assigned to LPT%d.\n",lptnum+1);
	break;
      case 3:
	printf("8x8 Touch screen assigned to LPT%d.\n",lptnum+1);
	break;
      default:
	break;
    }
    return (keypad_type);
}

void getkeypads()
// on exit lptnum, keypad_type are set
{
int numkeypads = 0;
int keypadsinstalled [] = {0,0,0};
int typekeypads [] = {0,0,0};
int i,kpt,key;

  /* determine if there are any keypads installed using display TSR */
  for (i = 0; i < 3; i++)
  {
    _AH = 3;
    _AL = 6;
    _DX = i;
    geninterrupt(itr_17);
    if ((_AX == 1) && (_BX == 2) && (_CX == 3))
    { // display tsr installed for this lpt
      kpt = printkeypad(i); // go check/print type of keypad installed (if any)
      if (kpt)
      {
	keypad_type = kpt;
	if (keypad_type)
	  numkeypads++;
	keypadsinstalled[i] = 1;
	typekeypads[i] = keypad_type;
	lptnum = i;
      }
    }
  }

  /* give option for which keypad if more than one exists */
  switch (numkeypads) {
  case 0:
    printf ("\nDISPLAY TSR is not installed or no keypad options used!\n");
    printf ("Install DISPLAY using DISPLAY Kn.\n");
    break;
  case 1:
    break;
  default:
    key = 0;
    while (!((key >= '1') && (key <= '3')))
    {
      printf("Which LPT is the keypad assigned to?");
      while (!kbhit());
      key = bioskey(0) & 0x7f; // get the key
      if ((key == 0x1b) || (key == 0x03))
	return;
      if (keypadsinstalled[key-'1'] == 0)
      {
	printf("\nNo keypad connected to LPT%d\n",key-'0');
	key = 0;
      }
    }
    keypad_type = typekeypads[key-'1'];
    lptnum = key-'1';
    printf("\nAccessing keypad connected to LPT%d.\n",lptnum+1);
  }
}

void main()
{
  printf("KPOFF disables DISPLAY TSRs keypad scanning.\n");
  getkeypads();
  KeypadScan(0);
  if (keypad_type)
    printf("Keypad scanning disabled.\n");
}

