 /*

        This software, including source code, is provided at no cost.  It
        is intended to be used for tutorial and demo purposes, and therefore,
        no warranty of any kind, express or implied is provided.  Under no
        circumstances whatsoever shall Octagon Systems Corporation be held
        liable for any damages, including any lost profits, lost savings,
	or other incidental or consequential damages arising from the use
        of, or inablity to use, this software.

	Filename:		KPTEST.CPP

	Function name:		main

	Received parameters:	none

	Returned parameters:	none

	Edit date:		09-02-94

	Function:		Tests the DISPLAY TSR Keypad support.

	Revisions:              Updated to CPP.	v3.5.0
				Added K3 type and
				updated for new display v40 09/02/94

*/

/* Includes */

#include	<stdio.h>
#include	<dos.h>
#include	<conio.h>
#include	<bios.h>

#define ESCAPE 0x1b
#define CTRLC  0x3

#define itr_17	0x17

char keychars_4x8[] = {"12345678ABCDEFGHIJKLMNOPQRSTUVWX"};
char keychars_4x4[] = {"123A456B789C*0#D"};

void printkey(unsigned char keypad_type, unsigned char keypsd)
{
  switch (keypad_type)
  {
    case 1:
      printf("%c  ",keychars_4x4[keypsd-1]);
      break;
    case 2:
      printf("%c  ",keychars_4x8[keypsd-1]);
      break;
    case 3:
      printf("%c  ",'A'+keypsd-1);
      break;
    default:
      break;
  }
}

void printkeydown(unsigned char keypad_type, unsigned char keypsd)
{
  if (keypsd)
  {
    switch (keypad_type)
    {
    case 1:
      printf("Key %c pressed\r",keychars_4x4[keypsd-1]);
      break;
    case 2:
      printf("Key %c pressed\r",keychars_4x8[keypsd-1]);
      break;
    case 3:
      printf("Key %c pressed\r",'A'+keypsd-1);
      break;
    default:
      break;
    }
  }
  else printf("                \r");

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


void main()
{
unsigned char last_keypsd,keypsd;
int num_keys,i,kpt,keypad_type,key;
int lptnum = 0;
int numkeypads = 0;
int keypadsinstalled [] = {0,0,0};
int typekeypads [] = {0,0,0};

  /* Print sign on message and instructions */
  printf("DISPLAY TSR keypad demonstration program v4.0\n");

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
    return;
  case 1:
    break;
  default:
    key = 0;
    while (!((key >= '1') && (key <= '3')))
    {
      printf("Which LPT is the test keypad assigned to?");
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
    printf("\nTesting keypad connected to LPT%d.\n",lptnum+1);
  }

  /* Do test of many of the keypad functions */
  printf("\nTest 1 - Test keypad entry.\n");
  printf("  Press a key on the keypad.\n");
  printf("  The pressed key will be displayed on the screen.\n");
  printf("  To continue with the next test, press any key on the keyboard.\n");
  printf("  Keypad press return values: ");

  while (! kbhit()) {
    /* if a key was pressed go get its value */
    _AH = 3;
    _AL = 1;
    _DX = lptnum;
    geninterrupt(itr_17);
    keypsd = _AH;
    if (keypsd)
      printkey (keypad_type, keypsd);
  } /* while */
  printf("\n\n");

  /* here to collect key that was hit and clean up the screen */
  key = bioskey(0) & 0x7f;
  if ((key == ESCAPE) || (key == CTRLC)) return;



  printf("Test 2 - Test debounce and repeat options\n");
  printf("  Press a key on the keypad.\n");
  printf("  The pressed key will be displayed on the screen.\n");
  printf("  If the key is held down it will be repeated.\n");
  printf("  Each key will have to be held down for 1/2 a second.\n");
  printf("  To continue with the next test, press any key on the keyboard.\n");
  printf("  Keypad press return values: ");

  _AH = 3;
  _AL = 5; /* repeat flag set */
  _BX = 1; /* repeat on */
  _DX = lptnum;
  geninterrupt(itr_17);

  _AH = 3;
  _AL = 4; /* set debounce count */
  _BX = 9; /* 1/2 second = 9 ticks */
  _DX = lptnum;
  geninterrupt(itr_17);

  while (! kbhit()) {
    /* check if there was a key pressed */
    _AH = 3;
    _AL = 0;
    _DX = lptnum;
    geninterrupt(itr_17);

    if (_AH) {
      /* if a key was pressed go get its value */
      _AH = 3;
      _AL = 1;
      _DX = lptnum;
      geninterrupt(itr_17);
      keypsd = _AH;
      printkey (keypad_type, keypsd);
    }

  } /* while */
  printf("\n\n");

  /* here to collect key that was hit and clean up the screen */
  key = bioskey(0) & 0x7f;
  if ((key == ESCAPE) || (key == CTRLC)) return;

  /* set debounce and repeat back to default */
  _AH = 3;
  _AL = 5; /* repeat flag set */
  _BX = 0; /* repeat off */
  _DX = lptnum;
  geninterrupt(itr_17);

  _AH = 3;
  _AL = 4; /* set debounce count */
  _BX = 1; /* 1/18 second = 1 ticks */
  _DX = lptnum;
  geninterrupt(itr_17);


  /* fill up the buffer then read the keys from the buffer */
  printf("Test 3 - test of keypad buffer\n");
  printf("  Press 8 known keys on the keypad.\n");
  printf("  Once all 8 keys are pressed all 8 keys are displayed.\n");

  num_keys = 0;
  while (! kbhit() & (num_keys != 8)) {
    /* check if there was a key pressed */
    _AH = 3;
    _AL = 0;
    _DX = lptnum;
    geninterrupt(itr_17);
    num_keys = _AH;
  }

  printf("  The keypad keys pressed were: ");

  for (i = 0;i < num_keys; i++) {
    _AH = 3;
    _AL = 1;
    _DX = lptnum;
    geninterrupt(itr_17);
    keypsd = _AH;
    if (keypsd)
      printkey(keypad_type, keypsd);
  }
  printf("\n\n");

  /* here to collect key that was hit and clean up the screen */
  if (bioskey(1)) {
    key = bioskey(0) & 0x7f;
    if ((key == ESCAPE) || (key == CTRLC)) return;
  }



  /* fill up the buffer then read the keys from the buffer */
  printf("Test 4 - Clear keypad buffer test\n");
  printf("  Press 8 known keys, after 4 keys are pressed the buffer is");
  printf(" cleared.\n");
  printf("  Therefore only the last 4 keys are shown.\n");

  num_keys = 0;
  while (! kbhit() & (num_keys != 4)) {
    /* check if there was a key pressed */
    _AH = 3;
    _AL = 0;
    _DX = lptnum;
    geninterrupt(itr_17);
    num_keys = _AH;
  }
  if (num_keys < 4) {
    key = bioskey(0) & 0x7f;
    if ((key == ESCAPE) || (key == CTRLC)) return;
  }
  else {
    _AH = 3;
    _AL = 3; /* clear keypad buffer */
    _DX = lptnum;
    geninterrupt(itr_17);

    /* now get the keys we want to display */
    num_keys = 0;
    while (! kbhit() & (num_keys != 4)) {
      /* check if there was a key pressed */
      _AH = 3;
      _AL = 0;
      _DX = lptnum;
      geninterrupt(itr_17);
      num_keys = _AH;
    }

    if (bioskey(1)) {
      key = bioskey(0) & 0x7f;
      if ((key == ESCAPE) || (key == CTRLC)) return;
    }

    printf("  The last 4 keys pressed were: ");
    for (i = 0;i < num_keys; i++) {
      _AH = 3;
      _AL = 1;
      _DX = lptnum;
      geninterrupt(itr_17);
      keypsd = _AH;
      if (keypsd)
	printkey(keypad_type, keypsd);
    }

    if (bioskey(1)) {
      key = bioskey(0) & 0x7f;
      if ((key == ESCAPE) || (key == CTRLC)) return;
    }
    printf("\n");
  }
  printf("\n");



  printf("Test 5 - Keypad key pressed test\n");
  printf("  Press any keypad key.\n");
  printf("  To exit the program press a key on the keyboard.\n");

  last_keypsd = 0;
  while (! kbhit()) {
    /* check if there was a key currently pressed */
    _AH = 3;
    _AL = 2;
    _DX = lptnum;
    geninterrupt(itr_17);
    keypsd = _AH;
    if (last_keypsd != keypsd)
      printkeydown(keypad_type, keypsd);
    last_keypsd = keypsd;
  } /* while */

  printf("\n\n");

  /* here to collect key that was hit and clean up the screen */
  if (bioskey(1)) {
    key = bioskey(0) & 0x7f;
    if ((key == ESCAPE) || (key == CTRLC)) return;
  }

  /* clean out the keypad buffer as well */
  _AH = 3;
  _AL = 3; /* clear buffer */
  _DX = lptnum;
  geninterrupt(itr_17);

}

